#include "Arduino.h"
#include <stdint.h>

#define PI 3.141592654

class FFT{
	public: 
	
	FFT(uint16_t samples, double sampleFreq);
	~FFT();
	void updateSamples(double* real, double*imag);
	void compute(); // computes complex FFT
	void getMags(); // complex to real magnitude
	double peak(); // get the most dominant frequency
	void window();
	void removeDC();
	
	private:
		
	double* real;
	double* imag;
	uint16_t samples;
	double sampleFreq;
	
	void swap(double& a, double& b);
	
//	void helper(double* realBuf, double* imagBuf, double* realOut, double* imagOut, uint16_t step);
};
