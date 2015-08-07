#include "Spectrometer.h"
#include <sys/stat.h>
#include <dirent.h>
#include <boost/filesystem.hpp>
#include <boost/regex.hpp>
#include <sstream>
#include <string>
#include <map>
#include <fstream>
#include <stdio.h>
#include <cstdio>
#include <ctime>

using namespace std;
using namespace boost;

//Constructor
Spectrometer::Spectrometer(string SerialNumber){
	
	bool DevNotFound = true;

	this->BasePath_ = "/sys/bus/usb/drivers/usbhspec/";
	//cout << "fixed BasePath!" << endl;
	//cout << this->BasePath_ << endl;
	struct stat sb;

	if (stat(this->BasePath_.c_str(), &sb) == -1) // if directory doesn't exist
	{
		cout << "No spectrometer found using the kernel module!" << endl;
		exit(EXIT_FAILURE);
	}
	
	this->Specs_=this->FindAllConnectedSpectrometers();
	this->PrintSpecs();


	if (this->SerialNumber_.empty())
	{
		this->SerialNumber_ = this->Specs_.begin()->first;
		//cout << "Ser.Nr: " << SerialNumber_ << endl;
		this->DeviceName_ = this->Specs_[SerialNumber_][0];
		//cout << "DevName: " << DeviceName_ << endl;
		stringstream path;
		path << "/sys/bus/usb/drivers/usbhspec/" << this->Specs_[SerialNumber_][1];
		this->BasePath_ = path.str();
		//cout << "basePath: " << BasePath_ << endl;
		DevNotFound=false;
	}

	if (DevNotFound)
	{
		cout << "Device not found! Exiting..." << endl;
		exit(EXIT_FAILURE);
	}

	stringstream path2;
	// First calibration coefficient of the spectrometer. Pixel counting starts at 1!
	path2 << this->BasePath_ << "/a0";
	this->StartWaveLength_ = atof(ReadContentOfFile(path2.str()).c_str());

	path2.str("");
	// Second calibration coefficient.
	path2 << this->BasePath_ << "/a1"; 
	this->FirstKoeff_ = atof(ReadContentOfFile(path2.str()).c_str());
	
	path2.str("");
	// Third calibration coefficient.
	path2 << this->BasePath_ << "/a2"; 
	this->SecondKoeff_ = atof(ReadContentOfFile(path2.str()).c_str());
	
	path2.str("");
	// Fourth calibration coefficient.
	path2 << this->BasePath_ << "/a3"; 
	this->ThirdKoeff_ = atof(ReadContentOfFile(path2.str()).c_str());

	path2.str("");
	// Fifth calibration coefficient.
	path2 << this->BasePath_ << "/a4"; 
	this->FourthKoeff_ = atof(ReadContentOfFile(path2.str()).c_str());

	path2.str("");
	// Sixth calibration coefficient.
	path2 << this->BasePath_ << "/a5"; 
	this->FifthKoeff_ = atof(ReadContentOfFile(path2.str()).c_str());

	path2.str("");


	path2 << this->BasePath_ << "/sensor_name";
	this->SensorName_ = ReadContentOfFile(path2.str());
	//cout << "SensorName_: " << SensorName_ << endl;

	path2.str("");

	path2 << "/dev/" << this->DeviceName_;
	this->DevicePath_ = path2.str();
	//cout << "DevicePath_: " << DevicePath_ << endl;

	this->PixelOffset_=0;

	//S10420-1006/-1106 CCD image sensor see documentation (Device structure)
	if (this->SensorName_ == "S10420-1106" || this->SensorName_ == "S10420-1006")
	{
		this->PixelOffset_ = 10;
		printf("Sensor '%s' means pixel offset of %f. \n", this->SensorName_.c_str(), this->PixelOffset_);
	}

	//int found1 = this->SensorName_.find("S8377");
	//int found2 = this->SensorName_.find("S8378");

	if (this->SensorName_.find("S8377") != string::npos || this->SensorName_.find("S8378") != string::npos)
	{
		this->PixelOffset_ = 0;
		printf("Sensor '%s' means pixel offset of %f. \n", this->SensorName_.c_str(), this->PixelOffset_);
	}

	// we need to get a spectrum before this->WLArr is filled!
	// what the heck, we take a spectrum and get this over with!
               
    this->IntegrationTime_ = 0;
    //cout << "IntegrationTime_: " << IntegrationTime_ << endl;
    this->SetIntegrationTime(10000);
    //cout << "IntegrationTime_: " << IntegrationTime_ << endl;
    this->GetSpectrum();

}

Spectrometer::~Spectrometer() {} //Destructor

string Spectrometer::GetSerialNumber(){
	return SerialNumber_;
}

string Spectrometer::GetDeviceName(){
	return DeviceName_;
}

string Spectrometer::GetBasePath(){
	return BasePath_;
}

map<string, vector<string>> Spectrometer::GetSpecs(){
	return Specs_;
}

int Spectrometer::GetIntegrationTime(){
    return IntegrationTime_;
}

float Spectrometer::GetStartWaveLength(){
    return StartWaveLength_;
}

float Spectrometer::GetFirstKoeff(){
	return FirstKoeff_;
}

float Spectrometer::GetSecondKoeff(){
	return SecondKoeff_;
}

float Spectrometer::GetThirdKoeff(){
	return ThirdKoeff_;
}

float Spectrometer::GetFourthKoeff(){
	return FourthKoeff_;
}

float Spectrometer::GetFifthKoeff(){
	return FifthKoeff_;
}

string Spectrometer::GetSensorName(){
	return SensorName_;
}

vector<double> Spectrometer::GetWLArr(){
	return WLArr_;
}

vector<unsigned int> Spectrometer::GetSpectrum(int TimeOut){
/*	This function grabs spectral data from the spectrometer. Addidtional
    information is also encoded in the data read from the interface so
    take nothing for granted!

    I've got two different spectrometers from which I tried to conclude
    what to do with the data. I may have made invalid assumptions!

    You have to read 0x1040 bytes of data. These are 16bit numbers so it does
    make sense to calculate these numbers from the data and the only use those
    integers.

    The first number (16 bit in this context) was alwas 2. That only changes
    if you read data if the spectrometer is not yet ready (integration time).
    If the spectrometer isn't ready all numbers are zero.

    The second number is an index which is 0,1 or 2 and just counts up.

    Third is the amount of active pixels (in contrast to the amount of pixels
    sixth number (index 5)). Please see the documentation to your image sensor
    (e.g. S10420-1106 from Hamamtsu).

    Forth (index 3) is the most important number because it says where to find
    the first pixel in the whole dataset of 0x1040/2 = 0x820 16bit numbers.

    The fifth seems to be the end index because startIndex + amountOfPixels = endIndex
    HOWEVER in my case the end index is higher than 0x820 which means that I can be
    very wrong with my interpretation.

    In my case and using the S10420-1106 sensor I found that the first four numbers
    of the spectrum (starting with startIndex) seem to be darkened pixels. You can
    increase the integration time as much as you wish these numbers stay (more or less)
    the same. Use at your own risc!

    Actually the spectrum seems to start at startIndex + 10 pixels/numbers which was
    veryfied for MY spectrometer with a really good and freshly checked laboratory
    spectrometer from Perkin Elmer (wavelength correctness was checked with the aid of
    special filters).
    So I calculate the start of the spectrum using
    startSpectrum = startIndex + (amountOfPixels - amountOfActivePixels) / 2
    which seems to make sense.

    The wavelength correctness was VERY good compared to spectrometers from avantes,
    ocean optics and trios. We estimat an offset of about .4 nm over the whole spectrum
    which lies in the tolerance of of our measurement setup.
    */

    if (TimeOut == 0)
    {
    	TimeOut = this->IntegrationTime_ / 1000000.0 * 8.0; // 2.1 was artificailly selected. 
    	//cout << "Timeout: " << TimeOut << endl;													// Keep in mind that that the aquisition of the catual spectrum may be with a higher integration time
    }

    double StartT = time(0); // seconds since epoch
    //cout << "StartT: " << StartT << endl;

    int whatever, Index0to2, AmountOfActivePixels, StartIndex, EndIndex, AmountOfPixels, SpectrumIndex;

    vector<unsigned int> ResArr = this->ReadFromDev(0x1040);
    //cout << "After Read from Dev" << endl;

    while (1) {

    	// Grab data from the interface

    	// I do not know what two means, but who cares. If this is not zero we have a spectrum!
    	whatever = ResArr[0];
    	//cout << "whatever: " << whatever << endl;
    	// Counts from 0 to 2 if you manage to get every spectrum
    	Index0to2 = ResArr[1];
    	//cout << "Index0to2: " << Index0to2 << endl;
    	// Amount of active pixels. See documentation of the sensor!
    	AmountOfActivePixels = ResArr[2];
    	//cout << "AmountOfActivePixels: " << AmountOfActivePixels << endl;
    	// Indicate where to find the first intensity of the spectral data. (Maybe with Offset)
    	StartIndex = ResArr[3];
    	//cout << "StartIndex: " << StartIndex << endl;
    	// Position of the last spectral data +1. This may lie out of the actual range of the data
    	EndIndex = ResArr[4];
    	//cout << "EndIndex: " << EndIndex << endl;
    	// Amount of pixels. See documentation of the sensor!
    	AmountOfPixels = ResArr[5];
    	//cout << "AmountOfPixels " << AmountOfPixels << endl;

    	// 6 & 7 are zero

    	// Seems to count all spectra taken since plugin. The Spectrometer takes spectra all the time.
    	SpectrumIndex = ResArr[8];


    	// if all values are zero no spectrum was received
    	if (ResArr.empty())
    	{
    		//cout << "ResArr is empty" << endl;
    		if (time(0) - StartT > TimeOut)
    		{
    			cout << "Timeout ... " << endl;
    			return ResArr;
    		}

    		usleep(2000); // sleep for 2 museconds
    		continue;
    	}

    	//////////////////////////////////////
        // data sanity check
        // we already know that there are values larger than 0 present
        // check if the first value is 2, index0to2 is between 0 and 2,
        // amountOfPixels is less than amountOfActivePixels, and
        // amountOfPixels = endIndex - startIndex

        int Offset =0;

        if (whatever == 3)
        {
        	cout << "Spectrometer has NO POWER (USB is connected -> use the power plug!)" << endl;
        	return ResArr;

        }

        if (!((whatever = 2) && (0 <= Index0to2) && !(Index0to2 <= 2) && ((AmountOfPixels = EndIndex - StartIndex || AmountOfPixels == EndIndex))))
        {
        	
        	while(Offset < ResArr.size()){

        		if (ResArr[Offset] == 2)
        		{
        			
        			Index0to2 = ResArr[Offset+1];
        			StartIndex = ResArr[Offset+3];
        			EndIndex = ResArr[Offset+4];
        			AmountOfPixels = ResArr[Offset+5];
        			if ((0 <= Index0to2) && (Index0to2 <= 2) && ((AmountOfPixels == EndIndex - StartIndex) || (AmountOfPixels == EndIndex)))
        			{
        				cout << "Offset = " << Offset << endl;
        				break;
        			}

        		}

        		Offset++;

        	}

        }

        if (Offset != 0)
        {
        	this->ReadFromDev(Offset*2);
        }

        else break;

    }

    int StartSpectrum = StartIndex + this->PixelOffset_;

    // Check if WLArr is already generated, if not generate it
    if (this->WLArr_.empty())
    {	
    	double wavelength;
    	for (int i = 0; i < AmountOfActivePixels; i++)
    	{
    		wavelength = this->StartWaveLength_ + i * this->FirstKoeff_
    											+ i*i * this->SecondKoeff_ 
    											+ i*i*i * this->ThirdKoeff_
    											+ i*i*i*i * this->FourthKoeff_
    											+ i*i*i*i*i * this->FifthKoeff_;
    		//cout << "wavelength: " << wavelength << endl;
    		WLArr_.push_back(wavelength);

    	}
    }

    /*for (unsigned int i = 0; i < WLArr_.size(); i++)
    {
    	cout << "WLArr: " << WLArr_[i] << endl;
    	cout << "ResArr: " << ResArr[StartSpectrum+i] << endl;
    }*/

    vector<unsigned int> ResArrSub(&ResArr[StartSpectrum],&ResArr[StartSpectrum+AmountOfActivePixels]);

    /* for (unsigned int i = 0; i < ResArrSub.size(); i++)
    {
    	cout << "ResArrSub: " << ResArrSub[i] << endl;
    }*/

    //cout << "WLArr_.size(): " << WLArr_.size() << " ResArr.size(): " << ResArr.size() << " ResArrSub.size(): " << ResArrSub.size() << endl;


    return ResArrSub;

}

vector<vector<string> > Spectrometer::GetDeviceFileFromAddress(string DevClass, string Address){

	//cout << "in GetDeviceFileFromAddress" << endl;
	vector<vector<string> > DevFileList;
	vector<string> v(2,"test");

	stringstream help;

	help << "/sys/class/" << DevClass << "/";
	//cout << help.str() << endl;

	string BasePath = help.str();
	struct dirent **namelist;

	int n = scandir(BasePath.c_str(), &namelist, NULL, alphasort);

	//cout << "after scandir" << endl;



	help.str("");
	//cout << "help sollte leer sein: " << endl;

	int i=0;
	while (i<n){
		//cout << "namelist: " << namelist[i]->d_name << endl;

		help << BasePath << namelist[i]->d_name;

		char buff[PATH_MAX];
		readlink(help.str().c_str(), buff, sizeof(buff)-1);

		//cout << "buff: " << buff << endl; 
		string buffstr(buff);
		//cout << buffstr << endl;

		//cout << "help: " << help.str() << " Adress: " << Address << endl;

		if (buffstr.find(Address) != string::npos)
		{
			v[0] = namelist[i]->d_name;
			v[1] = buffstr;
			//cout << "v0: " << v[0] << " " << "v1: " << v[1] << endl;
			DevFileList.push_back(v);
		}
		help.str("");
		i++;

	}


	return DevFileList;
}

vector<unsigned int> Spectrometer::ReadFromDev(int count){ // count normally is a hex number //approved

	//cout << "in ReadFromDev" << endl;
	FILE* f1 = fopen(this->DevicePath_.c_str(), "r");

	int n_chunks = 1;
	unsigned char* res = (unsigned char*) malloc (count * n_chunks);

	int result = fread(res, count, n_chunks, f1);

	//cout << "File read!" << endl;

	if (result != n_chunks){
		cout << "Reading error" << endl;
		fclose(f1);
		exit(EXIT_FAILURE);
	}

	fclose(f1);

	vector<unsigned int> resArr(count/2, 0);
	int index = 0;
	
	for (int i = 0; i < count; i=i+2)
	{
		resArr[index] = (res[i]) * 256 + (res[i+1]);

		//cout << "resArr[index] = " << resArr[index] << " res[i] = " << res[i] << " res[i+1] = " << res[i+1] << endl;
		//printf("output: %X %X %X\n", resArr[index], res[i], res[i+1]);

		index++;
	}

	return resArr;
}
 
map<string, vector<string>> Spectrometer::FindAllConnectedSpectrometers(){ //approved

	//Function to find all connected USB-spectrometers

    //Returns a list/dictionary? with spectrometers containing:
    //- device name
    //- serial number
    //- start wavelength

	map<string, vector<string>> specs;

	struct dirent **namelist;
	int n;

	// save the content of a directory to namelist
	n = scandir(this->BasePath_.c_str(), &namelist, NULL, alphasort);
	//cout << "Before regex" << endl;
	regex expression ("\\d-\\d:\\d\\.\\d"); //new spectrometer
	regex expression2 ("\\d-\\d\\.\\d:\\d\\.\\d"); //old spectrometer
	//cout << "Regex created" << endl;
	cmatch found;
	cmatch found2;
	bool match =false;
	bool match2 =false;
	int i=0;
	stringstream path1;
	stringstream path2;

	while(i<n){
		
		//cout << namelist[i]->d_name << endl;
		match=regex_match(namelist[i]->d_name, found, expression);
		match2=regex_match(namelist[i]->d_name, found2, expression2);
		 if (match || match2)		
		 {	
			//cout << "Match!: " << namelist[i]->d_name << endl;
		 	path1 << this->BasePath_ << namelist[i]->d_name << "/serial_number";
			//cout << path1.str() << endl;
		 	string serNr=ReadContentOfFile(path1.str());
			//cout << "Ser.Nr: " << serNr << endl;
		 	path2 << this->BasePath_ << namelist[i]->d_name << "/a0";
		 	vector<vector<string> > Res = GetDeviceFileFromAddress("usbmisc", namelist[i]->d_name);
		 	//cout << "Res00: " << Res[0][0] << endl;
		 	//cout << "after GetDeviceFileFromAddress" << endl;
		 	vector<string> content (3, "Null");
		 	content[0] = Res[0][0];
		 	content[1] = namelist[i]->d_name;
		 	content[2] = ReadContentOfFile(path2.str());
		 	specs[serNr]=content;
			path1.str("");
			path2.str("");
		 }
		i++;

	}

	return specs;
}

string Spectrometer::ReadContentOfFile(string path){ //approved

	string content;
	ifstream f1;
	f1.open(path);
	f1 >> content;

	return content;
}

void Spectrometer::PrintSpecs(){ //approved

	// loop over map
	for(auto const item : Specs_) {
		cout << item.first << " : ";
  		
  		for(auto const el : item.second) {
  			cout << el << " : ";  			
  		}
  		cout << endl;
	}
}

bool Spectrometer::SetIntegrationTime(int IntTime){

	if (IntTime<10000 || IntTime>10000000)
	{
		cout << "MiniSpec.setIntegrationTime : Integration Time not allowed! please use values between" << endl;
		cout << "10000 and 10000000µs" << endl;
		return false;
	}

	this->IntegrationTime_=IntTime;
	return true;

}

vector<vector<double> > Spectrometer::StartMeasurement(int IntTime, int NumberOfAverages){

	if(IntTime != this->IntegrationTime_){
		this->SetIntegrationTime(IntTime);
	}

	vector<unsigned int> data = this->GetSpectrum();

	int i = 1;
	while(i<NumberOfAverages){

		data = AddVector(data, this->GetSpectrum());
		i++;
	}

	for (unsigned int i = 0; i < data.size(); i++)
		{
			data[i]=data[i]/NumberOfAverages;
		}	
	
	vector<vector<double> > Result;

	if (data.empty())
	{
		return Result;
	}

	else{

		vector<double> data_double(data.size(),0);

		for (unsigned int j = 0; j < data.size(); j++)
		{	
			data_double[j]=(double)data[j];
		}
		Result[0] = WLArr_;
		Result[1] = data_double;
		return Result;

	}

}

vector<unsigned int> Spectrometer::AddVector(vector<unsigned int> a, vector<unsigned int> b){

	vector<unsigned int> result(a.size(), 0);

	if (a.size() == b.size())
	{
		for (unsigned int i = 0; i < a.size(); i++)
		{
			result[i] = a[i] + b[i];
		}

		return result;
	}

	else return result;
}




