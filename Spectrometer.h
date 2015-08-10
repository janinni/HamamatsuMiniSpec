//	Class to communicate with the spectrometer 
//
//	Created by Janine Müller on 30.07.2015
//
//	
//
//
//
//
//

#ifndef ____Spectrometer__
#define ____Spectrometer__

#include <vector>
#include <iostream>
#include <map>


class Spectrometer{

public:
	Spectrometer();
	Spectrometer(std::string SerialNumber); //Constructor
	~Spectrometer(); //Destructor
	std::map<std::string, std::vector<std::string>> FindAllConnectedSpectrometers();
	std::string ReadContentOfFile(std::string path);
	std::string GetSerialNumber();
	std::string GetDeviceName();
	std::string GetBasePath();
	std::string GetSensorName();
	std::map<std::string, std::vector<std::string>> GetSpecs();
	void PrintSpecs();
	bool SetIntegrationTime(int IntTime);
	int GetIntegrationTime();
	float GetStartWaveLength();
	float GetFirstKoeff();
	float GetSecondKoeff();
	float GetThirdKoeff();
	float GetFourthKoeff();
	float GetFifthKoeff();
	std::vector<double> GetWLArr();
	std::vector<unsigned int> GetSpectrum(int TimeOut=0);
	std::vector<unsigned int> ReadFromDev(int count);
	std::vector<std::vector<std::string>> GetDeviceFileFromAddress(std::string DevClass, std::string Address);
	std::vector<std::vector<double> > StartMeasurement(int IntTime, int NumberOfAverages=1);
	std::vector<unsigned int> AddVector(std::vector<unsigned int> a, std::vector<unsigned int> b);

private:

	std::string SerialNumber_;
	std::string DeviceName_;
	std::string BasePath_;
	std::map<std::string, std::vector<std::string>> Specs_;
	float StartWaveLength_;
	float FirstKoeff_;
	float SecondKoeff_;
	float ThirdKoeff_;
	float FourthKoeff_;
	float FifthKoeff_;
	std::string SensorName_;
	std::string DevicePath_;
	float PixelOffset_;
	std::vector<double> WLArr_;
	int IntegrationTime_;

};
    

#endif /* defined(____Spectrometer__) */
