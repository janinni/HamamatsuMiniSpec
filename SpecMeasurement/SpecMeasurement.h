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

#include "../Spectrometer.h"
#include "../../LEDClass/LED.h"
#include <sstream>
#include <string>
#include <fstream>
#include <vector>

using namespace std;

vector<unsigned int> AddVector(vector<unsigned int> a, vector<unsigned int> b);

class SpecMeasurement
{
	public:
		SpecMeasurement():_NumbOfAv(){};
		virtual ~SpecMeasurement(){};
		vector<vector<double> > SingleMeasurement(Spectrometer* ham);
		void SingleMeasurementWithDC(Spectrometer *ham, LED *led, string path);
		void Measurement3LWithDC(Spectrometer *ham, LED *led, double current1, double current2, double current3, string path);

		void SaveSingleMeasurement(vector<vector<double> > Result, string path);
		void SaveMeasurementDCand3L(vector<vector<double> > Result, vector<vector<double> > Result1, vector<vector<double> > Result2, vector<vector<double> > Result3, string path);
		void SaveMeasurementDCand1L(vector<vector<double> > Result, vector<vector<double> > Result1, string path);
		bool SetNumbOfAv(int n);
		int GetNumbOfAv();

	private:
		int _NumbOfAv = 5;
};



#endif /* defined(____SpecMeasurement__) */