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
#include "SpecMeasurement/SpecMeasurement.h"
#include "/home/xytable/src/LEDClass/LED.h"
#include <iostream>
#include <unistd.h>
#include <fstream>
#include <errno.h>
#include <signal.h>
#include <sstream>
#include <vector>

using namespace std;

// To do an action when somebody press Ctrl+C or if there is an error than interrupt the running of the code
void Error_handler(int sig) 
{
	cout << "" << endl;
	cout << "" << endl;
	cout << "It was terminated by a signal" << endl;

    // To print why it was terminated
    switch(sig) {
    	case SIGINT:
    		printf("Interrupt from keyboard; interactive attention signal.");
    		break;
    	case SIGTERM:
    		printf("Termination request.");
    		break;
    	case SIGABRT:
    		printf("Abnormal termination; abort signal from abort(3).");
    		break;
    	case SIGSEGV:
    		printf("„Segmentation violation“: invalid memory reference.");
    		break; 
    	default:
    		printf("Signal unknown"); 
    		break; 		
    }
    cout << "" << endl;
    cout << "" << endl;

    exit(1); // To shut down the program
}

int main()
{

	signal (SIGINT, Error_handler); 	// If the program is shut down using CTRL+C
	signal (SIGTERM, Error_handler); 	// If the program is shut down because of thermination request (kill)
	signal (SIGABRT, Error_handler); 	// If the program is shut down because of abnormal termination (?)
	signal (SIGSEGV, Error_handler); 	// If the program is shut down because of segmentation violation (the usual reason)

	Spectrometer *ham = new Spectrometer();
	//Spectrometer* ham2 = new Spectrometer();
	cout << "Spectrometers created!" << endl;

	LED *led = new LED();
	cout << "LED created!" << endl;

	SpecMeasurement *SpecMeas = new SpecMeasurement();

	double current1, current2, current3;
	
	char input;
	cout << "Do you want to take a 3 light measurement? (Y/N)" << endl;
	cin >> input;

	if((input=='y') | (input=='Y')){

		cout << "Which currents you would like to use? (current1, current2, current3 in mA)" << endl;
		cout << "Enter 1st current..." << endl;
		cin >> current1;
		cout << "Enter 2nd current..." << endl;
		cin >> current2;
		cout << "Enter 3rd current..." << endl;
		cin >> current3;

	}

	else if((input=='n') | (input=='N')){

		cout << "Which current you would like to use? (current in mA)" << endl;
		cin >> current1;

	}

	else{
		cout << "You should have entered N or Y!" << endl;
		exit(EXIT_FAILURE);
	}
	
	int IntTime;
	cout << "Which integration time you would like to use for the Spectrometer? (10000-10000000µs)" << endl;
	cin >> IntTime;

	int NumberOfAverages;
	cout << "How many averages would you like to take?" << endl;
	cin >> NumberOfAverages;


	if(SpecMeas->SetNumbOfAv(NumberOfAverages) && ham->SetIntegrationTime(IntTime)){
		cout << "You have 5 seconds to switch off the light" << endl;
		//sleep(5); 

		if((input=='y') | (input=='Y')){

			SpecMeas->Measurement3LWithDC(ham, led, current1, current2, current3, "/home/xytable/data/Spectrometer/TestSpectrum.txt");
			return 0;

		}

		else if((input=='n') | (input=='N')){

			led->SetCurr(current1);
			SpecMeas->SingleMeasurementWithDC(ham, led, "/home/xytable/data/Spectrometer/TestSpectrum.txt");
			return 0;

		}


		else exit(EXIT_FAILURE);
	}

	else exit(EXIT_FAILURE);

		
}	
























