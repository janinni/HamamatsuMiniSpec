#pragma once

#include <QWidget>
#include <QDoubleSpinBox>
#include <QComboBox>
#include <QPushButton>

// LED Class
#include "../../LEDClass/LED.h"
//Spectrometer Class
#include "../Spectrometer.h"
// Spectrometer Measurements
#include "../SpecMeasurement/SpecMeasurement.h"

class Spectrometer_App : public QWidget {
    
	Q_OBJECT

	public:
		Spectrometer_App(LED &Led, Spectrometer &Spec, SpecMeasurement &SpecMeas, QWidget *parent = 0);
		virtual ~Spectrometer_App() {};
		void SpecMeasChanged(QLineEdit *PathLineEdit, QComboBox *SpecMeasComboBox, QDoubleSpinBox *Current1SpinBox, QDoubleSpinBox *Current2SpinBox, QDoubleSpinBox *Current3SpinBox, QPushButton *Btn_Meas_on);

	private:
		LED &_Led;
		Spectrometer &_Spec;
		SpecMeasurement &_SpecMeas;

};