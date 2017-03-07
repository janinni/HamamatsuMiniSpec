#include "SpectrometerApp.h"
#include <iostream>
#include <QApplication>
#include <iostream>
#include <unistd.h>
#include <fstream>
#include <errno.h>
#include <signal.h>

// LED Class
#include "../../LEDClass/LED.h"
//Spectrometer Classes
#include "../Spectrometer.h"
#include "../SpecMeasurement/SpecMeasurement.h"



int main(int argc, char *argv[]) {
    
	QApplication app(argc, argv);

	// Spectrometer Spec;
	// LED Led;
	// SpecMeasurement SpecMeas;

	Spectrometer_App window;

	window.resize(1200, 300);
	window.setWindowTitle("Spectrometer Measurement");
	window.show();

	return app.exec();

	// Spectrometer ham;
	// //Spectrometer* ham2 = new Spectrometer();
	// cout << "Spectrometers created!" << endl;

	// LED led;
	// cout << "LED created!" << endl;

	// SpecMeasurement SpecMeas;

	// double current1, current2, current3;
	
	// char input;
	// cout << "Do you want to take a 3 light measurement? (Y/N)" << endl;
	// cin >> input;

	// if((input=='y') | (input=='Y')){

	// 	cout << "Which currents you would like to use? (current1, current2, current3 in mA)" << endl;
	// 	cout << "Enter 1st current..." << endl;
	// 	cin >> current1;
	// 	cout << "Enter 2nd current..." << endl;
	// 	cin >> current2;
	// 	cout << "Enter 3rd current..." << endl;
	// 	cin >> current3;

	// }

	// else if((input=='n') | (input=='N')){

	// 	cout << "Which current you would like to use? (current in mA)" << endl;
	// 	cin >> current1;

	// }

	// else{
	// 	cout << "You should have entered N or Y!" << endl;
	// 	exit(EXIT_FAILURE);
	// }
	
	// int IntTime;
	// cout << "Which integration time you would like to use for the Spectrometer? (10000-10000000µs)" << endl;
	// cin >> IntTime;

	// int NumberOfAverages;
	// cout << "How many averages would you like to take?" << endl;
	// cin >> NumberOfAverages;


	// if(SpecMeas.SetNumbOfAv(NumberOfAverages) && ham.SetIntegrationTime(IntTime)){
	// 	cout << "You have 5 seconds to switch off the light" << endl;
	// 	//sleep(5); 

	// 	if((input=='y') | (input=='Y')){

	// 		SpecMeas.Measurement3LWithDC(ham, led, current1, current2, current3, "/home/xytable/data/Spectrometer/TestSpectrum.txt");
	// 		return 0;

	// 	}

	// 	else if((input=='n') | (input=='N')){

	// 		led.SetCurr(current1);
	// 		SpecMeas.SingleMeasurementWithDC(ham, led, "/home/xytable/data/Spectrometer/TestSpectrum.txt");
	// 		return 0;

	// 	}


	// 	else exit(EXIT_FAILURE);
	// }

	// else exit(EXIT_FAILURE);
}