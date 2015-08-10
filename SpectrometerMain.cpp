//	Class to communicate with the spectrometer 
//
//	Created by Janine Müller on 30.07.2015
//
//	
//
//	Serialnumbers:
//	New Spectrometer: 615CAA01
//	Old Spectrometer: 
//

#include "Spectrometer.h"
#include <iostream>
#include <unistd.h>
#include <fstream>

using namespace std;

int main()
{
	Spectrometer* ham = new Spectrometer("615CAA01");
	//Spectrometer* ham2 = new Spectrometer();
	
	//cout << "Spectrometers created!" << endl;

	ham->Spectrometer::SetIntegrationTime(10000);
	//ham2->Spectrometer::SetIntegrationTime(10000);

	usleep(100000); //sleep 100ms = 0.1sec

	vector<unsigned int> Spectrum = ham->GetSpectrum();
	//vector<unsigned int> Spectrum2 = ham2->GetSpectrum();
	std::vector<double> WaveLength = ham->GetWLArr();
	//std::vector<double> WaveLength2 = ham2->GetWLArr();



	/*for (unsigned int i = 0; i < Spectrum.size(); i++)
	{
		cout << "Spectrum: " << Spectrum[i] << " WaveLength: " << WaveLength[i] << endl;
	}*/

	ofstream f1;

	f1.open("Spectrum.txt");

	if (Spectrum.size() != WaveLength.size())
	{
		cout << "Different sizes of Data and WaveLength!" << endl;
	}

	for (unsigned int i = 0; i < Spectrum.size(); i++)
	{
		f1 << WaveLength[i] << "\t" << Spectrum[i] << endl;//"\t" << WaveLength2[i] << "\t" << Spectrum2[i] << endl;
	}	

	return 0;	
}	
























