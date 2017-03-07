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
#include <QVector>
#include <QPen>

#include <iostream>
#include <functional>

// LED Class
#include "../../LEDClass/LED.h"
//Spectrometer Class
#include "../Spectrometer.h"
// Spectrometer Measurements
#include "../SpecMeasurement/SpecMeasurement.h"
// QCustom PLots
#include "../../../qcustomplot/qcustomplot.h"


using namespace std;

Spectrometer_App::Spectrometer_App(QWidget *parent):
  QWidget(parent),
	_Spec(new Spectrometer()),
	_SpecMeas(new SpecMeasurement()),
  _Led(new LED()),
  _Spec_x(),
  _Spec_y()

{

  //************* LED BOX *************//
  QGroupBox *LEDGroup = new QGroupBox(tr("LED"));

  // LED on
  QLabel *Current1Label = new QLabel(tr("Current1 / mA:"));
  QDoubleSpinBox *Current1SpinBox = new QDoubleSpinBox;
  Current1SpinBox->setRange(0., 20.);
  Current1SpinBox->setSingleStep(0.01);
  Current1SpinBox->setValue(0.00);
  this->_Led->SetCurr(0.00);
  QLabel *Current2Label = new QLabel(tr("Current2 / mA:"));
  QDoubleSpinBox *Current2SpinBox = new QDoubleSpinBox;
  Current2SpinBox->setRange(0., 20.);
  Current2SpinBox->setSingleStep(0.01);
  Current2SpinBox->setValue(0.00);
  this->_Led->SetCurr(0.00);
  QLabel *Current3Label = new QLabel(tr("Current3 / mA:"));
  QDoubleSpinBox *Current3SpinBox = new QDoubleSpinBox;
  Current3SpinBox->setRange(0., 20.);
  Current3SpinBox->setSingleStep(0.01);
  Current3SpinBox->setValue(0.00);
  this->_Led->SetCurr(0.00);
  
  Current1SpinBox->setEnabled( false );
  Current2SpinBox->setEnabled( false );
  Current3SpinBox->setEnabled( false );

  //DoubleSpin Box
  QObject::connect(Current1SpinBox, QOverload< double >::of(&QDoubleSpinBox::valueChanged), [=](double newValue) {
  this->_Led->SetCurr(newValue);
  });
    QObject::connect(Current2SpinBox, QOverload< double >::of(&QDoubleSpinBox::valueChanged), [=](double newValue) {
  this->_Led->SetCurr(newValue);
  });
      QObject::connect(Current3SpinBox, QOverload< double >::of(&QDoubleSpinBox::valueChanged), [=](double newValue) {
  this->_Led->SetCurr(newValue);
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
  this->_Spec->SetIntegrationTime(10000);

  // Spin Box
  QObject::connect(IntTimeSpinBox, QOverload< int >::of(&QSpinBox::valueChanged), [=](double newValue) {
  this->_Spec->SetIntegrationTime(newValue);
  });

  // Numb of Averages
  QLabel *NumbOfAvLabel = new QLabel(tr("Number of Averages:"));
  QSpinBox *NumbOfAvSpinBox = new QSpinBox;
  NumbOfAvSpinBox->setRange(1, 100);
  NumbOfAvSpinBox->setSingleStep(1);
  NumbOfAvSpinBox->setValue(5);
  this->_SpecMeas->SetNumbOfAv(5);

  // Spin Box
  QObject::connect(NumbOfAvSpinBox, QOverload< int >::of(&QSpinBox::valueChanged), [=](double newValue) {
  this->_SpecMeas->SetNumbOfAv(newValue);
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
  Btn_Meas_on->setEnabled(false);


  //************* Plot *************//
  QCustomPlot *Plot = new QCustomPlot();

  Plot->replot();

  // Reload Plot
  QPushButton *Btn_Plot = new QPushButton("Plot", this);
  Btn_Plot->setEnabled(false);
  connect(Btn_Plot, &QPushButton::clicked, [=]() {this->ReloadPlot(Plot) ;});

  // Connect Measurement Types
  connect( SpecMeasComboBox, QOverload< int >::of(&QComboBox::currentIndexChanged), [=]() {this->SpecMeasChanged(PathLineEdit, SpecMeasComboBox, Current1SpinBox, Current2SpinBox, Current3SpinBox, Btn_Meas_on, Btn_Plot);  });

  // Spectrometer action Grid //
  QGridLayout *SpecLayout = new QGridLayout;
  SpecLayout->addWidget(IntTimeLabel, 0, 0);
  SpecLayout->addWidget(IntTimeSpinBox, 1, 0);
  SpecLayout->addWidget(NumbOfAvLabel, 0, 1);
  SpecLayout->addWidget(NumbOfAvSpinBox, 1, 1);
  SpecLayout->addWidget(Plot, 0, 2, 7, 3);
  SpecLayout->addWidget(SpecMeasLabel, 2, 0, 1, 2);
  SpecLayout->addWidget(SpecMeasComboBox, 3, 0, 1, 2);
  SpecLayout->addWidget(PathLabel, 4, 0, 1, 2);
  SpecLayout->addWidget(PathLineEdit, 5, 0, 1, 2);
  SpecLayout->addWidget(Btn_Meas_on, 6, 0);
  SpecLayout->addWidget(Btn_Plot, 6, 1);
  SpecGroup->setLayout(SpecLayout);

  //************* QUIT *************//
  QPushButton *Btn_quit = new QPushButton("Quit", this);

  // Actions at Button Press
  connect(Btn_quit, &QPushButton::clicked, &QApplication::quit);

  //********** Put Layout together **********//        
  QGridLayout *layout = new QGridLayout;
  layout->addWidget(LEDGroup, 0, 1);
  layout->addWidget(SpecGroup, 1, 1);
  layout->addWidget(Btn_quit, 2, 1);
  setLayout(layout);

  setWindowTitle(tr("Spectrometer"));
}

void Spectrometer_App::ReloadPlot(QCustomPlot *Plot){

  this->_Spec_x = QVector<double>::fromStdVector(this->_SpecData[0]);
  this->_Spec_y = QVector<double>::fromStdVector(this->_SpecData[1]);

  for (unsigned int i = 0; i <this->_Spec_y.size(); i++)
	  {
	  	cout << this->_Spec_y[i] << endl;
	  }

  Plot->addGraph();
  // give the axes some labels:
  Plot->xAxis->setLabel("wavelength /nm");
  Plot->yAxis->setLabel("counts");
  // set axes ranges, so we see all data:
  Plot->xAxis->setRange(350, 700);
  Plot->yAxis->setRange(0, 60000);
  Plot->graph()->setLineStyle(QCPGraph::LineStyle::lsLine);
  // create graph and assign data to it:
  Plot->graph()->setData(this->_Spec_x, this->_Spec_y);
  Plot->graph()->rescaleAxes(true);
  Plot->replot();

}

// Disabled Current Spin Boxes depending on Measurement Mode
void Spectrometer_App::SpecMeasChanged(QLineEdit *PathLineEdit, QComboBox *SpecMeasComboBox, QDoubleSpinBox *Current1SpinBox, QDoubleSpinBox *Current2SpinBox, QDoubleSpinBox *Current3SpinBox, QPushButton *Btn_Meas_on, QPushButton *Btn_Plot) {
    if( SpecMeasComboBox->currentText() == "Dark + 1 Light" ){ 
      Current1SpinBox->setEnabled( true );
      Current2SpinBox->setEnabled( false );
      Current3SpinBox->setEnabled( false );

      Btn_Meas_on->setEnabled(true);

      connect(Btn_Meas_on, &QPushButton::clicked, [=]() {this->_SpecMeas->SingleMeasurementWithDC(this->_Spec, this->_Led, PathLineEdit->text().toStdString());  });
      //this->_SpecMeas->SingleMeasurementWithDC(this->_Spec, this->_Led, "/home/xytable/data/Spectrometer/TestSpectrum.txt");
    }

    else if( SpecMeasComboBox->currentText() == "Single" ){ 
      Current1SpinBox->setEnabled( false );
      Current2SpinBox->setEnabled( false );
      Current3SpinBox->setEnabled( false );

      Btn_Meas_on->setEnabled(true);

      Btn_Plot->setEnabled( true );
      connect(Btn_Meas_on, &QPushButton::clicked, [=]() {
        //this->_Led->LEDon();
        //cout << this->_SpecMeas->GetNumbOfAv() << endl;
        this->_SpecData = this->_SpecMeas->SingleMeasurement(this->_Spec);
        //this->_Led->LEDoff();
      });

    }

    else if( SpecMeasComboBox->currentText() == "Dark + 3 Light" ){ 
      Current1SpinBox->setEnabled( true );
      Current2SpinBox->setEnabled( true );
      Current3SpinBox->setEnabled( true );

      Btn_Meas_on->setEnabled(true);

      connect(Btn_Meas_on, &QPushButton::clicked, [=]() {this->_SpecMeas->Measurement3LWithDC(this->_Spec, this->_Led, Current1SpinBox->value(), Current2SpinBox->value(), Current3SpinBox->value(), PathLineEdit->text().toStdString());  });

    }
    else{
      Current1SpinBox->setEnabled( false );
      Current2SpinBox->setEnabled( false );
      Current3SpinBox->setEnabled( false );
    }
}