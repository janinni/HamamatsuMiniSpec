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

#include "Spectrometer.h"
#include <sstream>
#include <string>
#include <fstream>
#include <vector>

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

vector<vector<double> > StartMeasurement(Spectrometer *ham, int IntTime, int NumberOfAverages = 1){

	if(IntTime != ham->GetIntegrationTime()){
		ham->SetIntegrationTime(IntTime);
	}

	//cout << "IntegrationTime: " << ham->GetIntegrationTime() << endl;

	vector<unsigned int> data = ham->GetSpectrum();
	/*for (unsigned int i = 0; i < data.size(); i++)
	{
		cout << "data: " << data[i] << endl;
	}*/

	int i = 1;
	vector<unsigned int> help;
	while(i<NumberOfAverages){

		help = ham->GetSpectrum();
		//cout << "i: " << i << endl;
		data = AddVector(data, help);
		i++;
	}
	
	/*for (unsigned int i = 0; i < data.size(); i++)
	{
		cout << "after sum up data: " << data[i] << endl;
	}*/

	vector<double> data_double(data.size(),0);

	for (unsigned int j = 0; j < data.size(); j++)
	{	
		data_double[j]=(double)data[j];
		//cout << "data_double: " << data_double[j] << endl;
	}

	for (unsigned int i = 0; i < data_double.size(); i++)
		{
			data_double[i]=data_double[i]/NumberOfAverages;
			//cout << "data_double_avg: " << data_double[j] << endl;
		}	

	vector<vector<double> > Result;

	if (data_double.empty())
	{	
		cout << "data empty" << endl;
		return Result;
	}

	else{

		Result.push_back(ham->GetWLArr());
		Result.push_back(data_double);
		return Result;

	}

}

void SaveMeasurement(vector<vector<double> > Result, string path){

	ofstream f1;

	f1.open(path.c_str());

	for (unsigned int i = 0; i < Result[0].size(); i++)
	{
		f1 << Result[0][i] << "\t" << Result[1][i] << endl;
	}

	f1.close();
}

void SaveMeasurementDL(vector<vector<double> > Result, vector<vector<double> > Result1, vector<vector<double> > Result2, vector<vector<double> > Result3, string path){

	ofstream f1;

	f1.open(path.c_str());

	for (unsigned int i = 0; i < Result[0].size(); i++)
	{
		// 	  wavelength---------------dark count------light 1----------light 2---------light 3------ 
		f1 << Result[0][i] << "\t" << Result[1][i] << "\t" << Result1[1][i] << "\t" << Result2[1][i] << "\t" << Result3[1][i] << endl;
	}

	f1.close();
}