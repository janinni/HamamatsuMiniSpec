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

	Spectrometer_App window;

	window.resize(1200, 300);
	window.setWindowTitle("Spectrometer Measurement");
	window.show();

	return app.exec();

}