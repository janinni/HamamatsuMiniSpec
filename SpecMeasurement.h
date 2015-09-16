//	Function to support the SpectrometerClass
//
//	created by Janine Müller on 30.07.2015
//
//	
//
//
//
//
//

#ifndef ____SpecMeasurement__
#define ____SpecMeasurement__

#include "Spectrometer.h"
#include <sstream>
#include <string>
#include <fstream>
#include <vector>

using namespace std;

vector<unsigned int> AddVector(vector<unsigned int> a, vector<unsigned int> b);
vector<vector<double> > StartMeasurement(Spectrometer* ham, int IntTime, int NumberOfAverages = 1);
void SaveMeasurement(vector<vector<double> > Result, string path);
void SaveMeasurementDL(vector<vector<double> > Result, vector<vector<double> > Result1, vector<vector<double> > Result2, vector<vector<double> > Result3, string path);
void SaveMeasurementD1L(vector<vector<double> > Result, vector<vector<double> > Result1, string path);

#endif /* defined(____SpecMeasurement__) */