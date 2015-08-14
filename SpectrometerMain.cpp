//	Class to communicate with the spectrometer 
//
//	Created by Janine Müller on 30.07.2015
//
//	
//
//	Serialnumbers:
//	New Spectrometer: 615CAA01
//	Old Spectrometer: 114CAA01
//

#include "Spectrometer.h"
#include "SpecMeasurement.h"
#include "/home/laborlinux/src/LED/LED.h"
#include <iostream>
#include <unistd.h>
#include <fstream>

using namespace std;

int main()
{
	Spectrometer* ham = new Spectrometer();
	//Spectrometer* ham2 = new Spectrometer();
	
	cout << "Spectrometers created!" << endl;

	int IntTime;
	cout << "Which integration time you would like to use? (10000-10000000µs)" << endl;
	cin >> IntTime;

	int NumberOfAverages;
	cout << "How many averages would you like to take?" << endl;
	cin >> NumberOfAverages;

	double current1;
	cout << "Which currents you would like to use? (current1, current2, current3, in mA)" << endl;
	cin >> current1;

	ham->SetIntegrationTime(IntTime);
	//ham2->Spectrometer::SetIntegrationTime(10000);

	LEDon(current1);

	sleep(2); //sleep 100ms = 0.1sec

	vector<vector<double> > Result = StartMeasurement(ham, IntTime, NumberOfAverages);

	SaveMeasurement(Result, "/home/laborlinux/Data/Spectrometer/TestSpectrum.txt");

	LEDoff();

	return 0;	
}	
























