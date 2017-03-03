#include "SpectrometerApp.h"

// QT stuff
#include <QtGlobal>
#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QDoubleSpinBox>
#include <QWidget>
#include <QApplication>
#include <QPushButton>
#include <QComboBox>
#include <QLineEdit>
#include <iostream>
#include <functional>

// LED Class
#include "../../LEDClass/LED.h"
//Spectrometer Class
#include "../Spectrometer.h"
// Spectrometer Measurements
#include "../SpecMeasurement/SpecMeasurement.h"


using namespace std;

Spectrometer_App::Spectrometer_App(LED &Led, Spectrometer &Spec, SpecMeasurement &SpecMeas, QWidget *parent):
	_Led(Led),
	_Spec(Spec),
	_SpecMeas(SpecMeas),
	QWidget(parent)
  
{
  //************* LED BOX *************//
  QGroupBox *LEDGroup = new QGroupBox(tr("LED"));

  // LED on
  QLabel *Current1Label = new QLabel(tr("Current1 / mA:"));
  QDoubleSpinBox *Current1SpinBox = new QDoubleSpinBox;
  Current1SpinBox->setRange(-20., 20.);
  Current1SpinBox->setSingleStep(0.01);
  Current1SpinBox->setValue(0.00);
  QLabel *Current2Label = new QLabel(tr("Current2 / mA:"));
  QDoubleSpinBox *Current2SpinBox = new QDoubleSpinBox;
  Current2SpinBox->setRange(-20., 20.);
  Current2SpinBox->setSingleStep(0.01);
  Current2SpinBox->setValue(0.00);
  QLabel *Current3Label = new QLabel(tr("Current3 / mA:"));
  QDoubleSpinBox *Current3SpinBox = new QDoubleSpinBox;
  Current3SpinBox->setRange(-20., 20.);
  Current3SpinBox->setSingleStep(0.01);
  Current3SpinBox->setValue(0.00);

  Current1SpinBox->setEnabled( false );
  Current2SpinBox->setEnabled( false );
  Current3SpinBox->setEnabled( false );

  //DoubleSpin Box
  QObject::connect(Current1SpinBox, QOverload< double >::of(&QDoubleSpinBox::valueChanged), [=](double newValue) {
  _Led.SetCurr(newValue);
  });
    QObject::connect(Current2SpinBox, QOverload< double >::of(&QDoubleSpinBox::valueChanged), [=](double newValue) {
  _Led.SetCurr(newValue);
  });
      QObject::connect(Current3SpinBox, QOverload< double >::of(&QDoubleSpinBox::valueChanged), [=](double newValue) {
  _Led.SetCurr(newValue);
  });

  QGridLayout *LEDLayout = new QGridLayout;
  LEDLayout->addWidget(Current1Label, 0, 0);
  LEDLayout->addWidget(Current1SpinBox, 0, 1);
  LEDLayout->addWidget(Current2Label, 1, 0);
  LEDLayout->addWidget(Current2SpinBox, 1, 1);
  LEDLayout->addWidget(Current3Label, 2, 0);
  LEDLayout->addWidget(Current3SpinBox, 2, 1);
  LEDGroup->setLayout(LEDLayout);


  //************* Spectrometer BOX *************//
  QGroupBox *SpecGroup = new QGroupBox(tr("Spectrometer"));

  // Int Time
  QLabel *IntTimeLabel = new QLabel(tr("Integration Time / µs:"));
  QSpinBox *IntTimeSpinBox = new QSpinBox;
  IntTimeSpinBox->setRange(10000, 10000000);
  IntTimeSpinBox->setSingleStep(1);
  IntTimeSpinBox->setValue(10000);

  // Spin Box
  QObject::connect(IntTimeSpinBox, QOverload< int >::of(&QSpinBox::valueChanged), [=](double newValue) {
  _Spec.SetIntegrationTime(newValue);
  });

  // Numb of Averages
  QLabel *NumbOfAvLabel = new QLabel(tr("Number of Averages:"));
  QSpinBox *NumbOfAvSpinBox = new QSpinBox;
  NumbOfAvSpinBox->setRange(1, 100);
  NumbOfAvSpinBox->setSingleStep(1);
  NumbOfAvSpinBox->setValue(5);

  // Spin Box
  QObject::connect(NumbOfAvSpinBox, QOverload< int >::of(&QSpinBox::valueChanged), [=](double newValue) {
  this->_SpecMeas.SetNumbOfAv(newValue);
  });

  // Measurement Mode
  QLabel *SpecMeasLabel = new QLabel(tr("Measurement Mode:"));
  QComboBox *SpecMeasComboBox = new QComboBox;
  SpecMeasComboBox->addItem(tr("<Select Mode>"));
  SpecMeasComboBox->addItem(tr("Single"));
  SpecMeasComboBox->addItem(tr("Dark + 1 Light"));
  SpecMeasComboBox->addItem(tr("Dark + 3 Light"));

  // Path
  QLabel *PathLabel = new QLabel(tr("Filename:"));
  QLineEdit *PathLineEdit = new QLineEdit;
  //PathLineEdit->setPlaceholderText("Placeholder Text");
  PathLineEdit->setText("../Test.txt");


  // Spec Meas ON Button
  QPushButton *Btn_Meas_on = new QPushButton("Start Measurement", this);

  connect( SpecMeasComboBox, QOverload< int >::of(&QComboBox::currentIndexChanged), [=]() {SpecMeasChanged(PathLineEdit, SpecMeasComboBox, Current1SpinBox, Current2SpinBox, Current3SpinBox, Btn_Meas_on);  });

  QGridLayout *SpecLayout = new QGridLayout;
  SpecLayout->addWidget(IntTimeLabel, 0, 0);
  SpecLayout->addWidget(IntTimeSpinBox, 1, 0);
  SpecLayout->addWidget(NumbOfAvLabel, 0, 1);
  SpecLayout->addWidget(NumbOfAvSpinBox, 1, 1);
  SpecLayout->addWidget(SpecMeasLabel, 2, 0, 1, 2);
  SpecLayout->addWidget(SpecMeasComboBox, 3, 0, 1, 2);
  SpecLayout->addWidget(PathLabel, 4, 0, 1, 2);
  SpecLayout->addWidget(PathLineEdit, 5, 0, 1, 2);
  SpecLayout->addWidget(Btn_Meas_on, 6, 0, 1, 2);
  SpecGroup->setLayout(SpecLayout);

  //************* QUIT *************//
  QPushButton *Btn_quit = new QPushButton("Quit", this);
  
  // Put Layout together //        
  QGridLayout *layout = new QGridLayout;
  layout->addWidget(LEDGroup, 0, 1);
  layout->addWidget(SpecGroup, 1, 1);
  layout->addWidget(Btn_quit, 2, 1);
  setLayout(layout);

  setWindowTitle(tr("Spectrometer"));

  // Actions at Button Press
  connect(Btn_quit, &QPushButton::clicked, &QApplication::quit);
}

// Disabled Current Spin Boxes depending on Measurement Mode
void Spectrometer_App::SpecMeasChanged(QLineEdit *PathLineEdit, QComboBox *SpecMeasComboBox, QDoubleSpinBox *Current1SpinBox, QDoubleSpinBox *Current2SpinBox, QDoubleSpinBox *Current3SpinBox, QPushButton *Btn_Meas_on) {
    if( SpecMeasComboBox->currentText() == "Dark + 1 Light" ){ 
      Current1SpinBox->setEnabled( true );
      Current2SpinBox->setEnabled( false );
      Current3SpinBox->setEnabled( false );

      connect(Btn_Meas_on, &QPushButton::clicked, [=]() {this->_SpecMeas.SingleMeasurementWithDC(this->_Spec, this->_Led, PathLineEdit->text().toStdString());  });
      //this->_SpecMeas->SingleMeasurementWithDC(this->_Spec, this->_Led, "/home/xytable/data/Spectrometer/TestSpectrum.txt");
    }

    else if( SpecMeasComboBox->currentText() == "Single" ){ 
      Current1SpinBox->setEnabled( false );
      Current2SpinBox->setEnabled( false );
      Current3SpinBox->setEnabled( false );

      connect(Btn_Meas_on, &QPushButton::clicked, [=]() {this->_SpecMeas.SingleMeasurement(this->_Spec);  });

    }

    else if( SpecMeasComboBox->currentText() == "Dark + 3 Light" ){ 
      Current1SpinBox->setEnabled( true );
      Current2SpinBox->setEnabled( true );
      Current3SpinBox->setEnabled( true );

      connect(Btn_Meas_on, &QPushButton::clicked, [=]() {this->_SpecMeas.Measurement3LWithDC(this->_Spec, this->_Led, Current1SpinBox->value(), Current2SpinBox->value(), Current3SpinBox->value(), PathLineEdit->text().toStdString());  });

    }
    else{
      Current1SpinBox->setEnabled( false );
      Current2SpinBox->setEnabled( false );
      Current3SpinBox->setEnabled( false );
    }
}