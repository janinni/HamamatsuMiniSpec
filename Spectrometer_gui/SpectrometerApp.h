#pragma once

#include <QWidget>
#include <QDoubleSpinBox>
#include <QComboBox>
#include <QPushButton>
#include <QLineEdit>
#include <QVector>

// LED Class
#include "../../LEDClass/LED.h"
//Spectrometer Class
#include "../Spectrometer.h"
// Spectrometer Measurements
#include "../SpecMeasurement/SpecMeasurement.h"
// QCustom Plots
#include "../../../qcustomplot/qcustomplot.h"

class Spectrometer_App : public QWidget {
    
	Q_OBJECT

	public:
		Spectrometer_App(QWidget *parent = 0);
		virtual ~Spectrometer_App() {};
		void SpecMeasChanged(QStackedWidget *MeasButton, QLineEdit *PathLineEdit, QComboBox *SpecMeasComboBox, QDoubleSpinBox *Current1SpinBox, QDoubleSpinBox *Current2SpinBox, QDoubleSpinBox *Current3SpinBox, QPushButton *Btn_Plot);
		void ReloadPlot(QCustomPlot *Plot);

	private:
		Spectrometer *_Spec;
		SpecMeasurement *_SpecMeas;
		LED *_Led;
		QVector<double> _Spec_x; 
		QVector<double> _Spec_y;
		vector<vector<double> > _SpecData;
};
