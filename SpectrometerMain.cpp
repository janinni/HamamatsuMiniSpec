//
//
//
//
//

#include "Spectrometer.h"
#include <iostream>
#include <unistd.h>
#include <fstream>

using namespace std;

int main()
{
	Spectrometer* ham = new Spectrometer("ABCDEF23");
	
	cout << "Spectrometer created!" << endl;

	ham->Spectrometer::SetIntegrationTime(10000);

	usleep(100000); //sleep 100ms = 0.1sec

	vector<unsigned int> Spectrum = ham->GetSpectrum();
	std::vector<double> WaveLength = ham->GetWLArr();

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
		f1 << WaveLength[i] << "\t" << Spectrum[i] << endl;
	}	

	return 0;	
}	
























