//	Function to support the SpectrometerClass
//
//	created by Janine Müller on 30.07.2015
//
//	
//
//
//
//
//

#include "../Spectrometer.h"
#include "../../LEDClass/LED.h"
#include "SpecMeasurement.h"
#include <sstream>
#include <string>
#include <fstream>
#include <vector>
#include <unistd.h>
#include <signal.h>	// To see if somedoy do Ctrl+C (and the ones under)

using namespace std;

vector<unsigned int> AddVector(vector<unsigned int> a, vector<unsigned int> b){

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

bool SpecMeasurement::SetNumbOfAv(int n){

	if (n<0 || n>1000)
	{
		cout << "Negative Average or larger then 1000 not possible" << endl;
		return false;
	}

	else{

		this->_NumbOfAv=n;
		return true;
	}

}

int SpecMeasurement::GetNumbOfAv(){
	return this->_NumbOfAv;
}

vector<vector<double> > SpecMeasurement::SingleMeasurement(Spectrometer *ham){

	vector<unsigned int> data = ham->GetSpectrum();
	for (unsigned int i = 0; i < data.size(); i++)
	{
		//cout << "data: " << data[i] << endl;
	}
	int i = 1;
	vector<unsigned int> help;
	while(i <= this->_NumbOfAv){

		help = ham->GetSpectrum();
		//cout << "i: " << i << endl;
		data = AddVector(data, help);
		i++;
	}

	// for (unsigned int i = 0; i < data.size(); i++)
	// {
	// 	cout << "after sum up data: " << data[i] << endl;
	// }

	vector<double> data_double(data.size(),0);

	for (unsigned int j = 0; j < data.size(); j++)
	{	
		data_double[j]=(double)data[j];
		//cout << "data_double: " << data_double[j] << endl;
	}

	for (unsigned int i = 0; i < data_double.size(); i++)
		{
			data_double[i]=data_double[i]/this->_NumbOfAv;
			//cout << "data_double_avg: " << data_double[i] << endl;
		}	

	vector<vector<double> > Result;

	if (data_double.empty())
	{	
		cout << "data empty" << endl;
		return Result;
	}

	else{
		// vector<double> Wlarr = ham->GetWLArr();
		// for(unsigned i =0; i<Wlarr.size(); i++){
		// 	cout << Wlarr[i] << endl;
		// }


		Result.push_back(ham->GetWLArr());
		Result.push_back(data_double);
		//cout << "Data fetched" << endl;
		return Result;

	}



}

void SpecMeasurement::SingleMeasurementWithDC(Spectrometer *ham, LED *led, string path){

	// Dark Count Measurement
	cout << "Dark Count Measurement ongoing..." << endl;
	vector<vector<double> > ResultD = this->SingleMeasurement(ham);

	sleep(3);

	if (led->GetCurr() == 0)
	{
		led->LEDoff();
	}

	else
	{
		led->LEDon();
	}

	sleep(5); 

	vector<vector<double> > ResultL = this->SingleMeasurement(ham);
	
	led->LEDoff();

	this->SaveMeasurementDCand1L(ResultD, ResultL, path);

}

void SpecMeasurement::Measurement3LWithDC(Spectrometer *ham, LED *led, double current1, double current2, double current3, string path){

	// Dark Count Measurement
	cout << "Dark Count Measurement ongoing..." << endl;
	vector<vector<double> > ResultD = this->SingleMeasurement(ham);

	// 1st measurement
	led->SetCurr(current1);
	if (led->GetCurr() == 0)
	{
		led->LEDoff();
	}

	else
	{
		led->LEDon();
	}

	sleep(2); 

	vector<vector<double> > ResultL1 = this->SingleMeasurement(ham);

	// 2nd measurement
	led->SetCurr(current2);
	if (led->GetCurr() == 0)
	{
		led->LEDoff();
	}

	else
	{
		led->LEDon();
	}

	sleep(2); 

	vector<vector<double> > ResultL2 = this->SingleMeasurement(ham);

	// 3rd measurement
	led->SetCurr(current3);
	if (led->GetCurr() == 0)
	{
		led->LEDoff();
	}

	else
	{
		led->LEDon();
	}

	sleep(2); 

	vector<vector<double> > ResultL3 = this->SingleMeasurement(ham);
	
	led->LEDoff();

	this->SaveMeasurementDCand3L(ResultD, ResultL1, ResultL2, ResultL3, path);

}

void SpecMeasurement::SaveSingleMeasurement(vector<vector<double> > Result, string path){

	ofstream f1;

	if (path == ""){
		cout << "Use default path: " << this->_path << endl;
		path = this->_path;
	}

	try{
		f1.open(path.c_str());
	}
	catch (std::ofstream::failure &writeErr) {
  	cout << "Exception opening/reading file" << endl;
	}
	
	if(f1){

		f1 << "#lambda/nm" << "\t" << "C1" << endl;

		for (unsigned int i = 0; i < Result[0].size(); i++)
		{
			f1 << Result[0][i] << "\t" << Result[1][i] << endl;
		}

		f1.close();

		cout << "Data saved at " << path << endl;

	}

	else cout << "Exception opening/reading file" << endl;

}

void SpecMeasurement::SaveMeasurementDCand3L(vector<vector<double> > Result, vector<vector<double> > Result1, vector<vector<double> > Result2, vector<vector<double> > Result3, string path){

	ofstream f1;

	if (path == ""){
		cout << "Use default path: " << this->_path << endl;
		path = this->_path;
	}

	try{
		f1.open(path.c_str());
	}
	catch (std::ofstream::failure &writeErr) {
  	cout << "Exception opening/reading file" << endl;
	}
	
	if(f1){

		f1 << "#lambda/nm" << "\t" << "DC" << "\t" << "C1" << "\t" << "C2" << "\t" << "C3" << endl;

		for (unsigned int i = 0; i < Result[0].size(); i++)
		{
			// 	  wavelength---------------dark count------light 1----------light 2---------light 3------ 
			f1 << Result[0][i] << "\t" << Result[1][i] << "\t" << Result1[1][i] << "\t" << Result2[1][i] << "\t" << Result3[1][i] << endl;
		}

		f1.close();
		cout << "Data saved at " << path << endl;

	}

	else cout << "Exception opening/reading file" << endl;
	
}

void SpecMeasurement::SaveMeasurementDCand1L(vector<vector<double> > Result, vector<vector<double> > Result1, string path){

	ofstream f1;

	if (path == ""){
		cout << "Use default path: " << this->_path << endl;
		path = this->_path;
	}

	try{
		f1.open(path.c_str());
	}
	catch (std::ofstream::failure &writeErr) {
  	cout << "Exception opening/reading file" << endl;
	}
	
	if(f1){

		f1 << "#lambda/nm" << "\t" << "DC" << "\t" << "C1" << endl;

		for (unsigned int i = 0; i < Result[0].size(); i++)
		{
			// 	  wavelength---------------dark count----------------light 1-----------
			f1 << Result[0][i] << "\t" << Result[1][i] << "\t" << Result1[1][i] << endl;
		}

		f1.close();
		cout << "Data saved at " << path << endl;
	}

	else cout << "Exception opening/reading file" << endl;
}