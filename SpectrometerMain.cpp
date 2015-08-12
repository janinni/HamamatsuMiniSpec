//	Class to communicate with the spectrometer 
//
//	Created by Janine Müller on 30.07.2015
//
//	
//
//	Serialnumbers:
//	New Spectrometer: 615CAA01
//	Old Spectrometer: 
//

#include "Spectrometer.h"
#include "SpecMeasurement.h"
#include <iostream>
#include <unistd.h>
#include <fstream>

using namespace std;

int main()
{
	Spectrometer* ham = new Spectrometer();
	//Spectrometer* ham2 = new Spectrometer();
	
	cout << "Spectrometers created!" << endl;

	ham->SetIntegrationTime(10000);
	//ham2->Spectrometer::SetIntegrationTime(10000);

	usleep(100000); //sleep 100ms = 0.1sec

	vector<vector<double> > Result = StartMeasurement(ham, 10000, 4);

	SaveMeasurement(Result, "/home/laborlinux/Data/Spectrometer");

	return 0;	
}	
























