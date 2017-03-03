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

	Spectrometer Spec;
	LED Led;
	SpecMeasurement SpecMeas;
	    
	Spectrometer_App window(Led, Spec, SpecMeas);

	window.resize(300, 190);
	window.setWindowTitle("Spectrometer Measurement");
	window.show();

	return app.exec();
}