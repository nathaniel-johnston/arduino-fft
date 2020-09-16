#include "FFT.h"


FFT::FFT(uint16_t samples, double sampleFreq) {
	this->real = new double[samples];
	this->imag = new double[samples];
	this->samples = samples;
	this->sampleFreq = sampleFreq;
}

FFT::~FFT() {
	delete[] this->real;
	delete[] this->imag;
}

void FFT::window() {
	double weight = 0.0;
	double doubleSamples = double(this->samples) - 1.0;
	
	for(uint16_t i = 0; i < this->samples >> 1; i++) {
		weight = 0.35875 - (0.48829*cos(2.0*PI*i/doubleSamples)) + (0.14128 *cos(4.0*PI*i/doubleSamples)) - (0.01168*cos(6.0*PI*i/doubleSamples));
		
		this->real[i] *= weight;
		this->real[this->samples - i - 1] *= weight;
	}
}

void FFT::removeDC() {
	double avg = 0.0;
	
	for(uint16_t i = 0; i < this->samples; i++) {
		avg += this->real[i]/this->samples;
	}
	
	for(uint16_t i = 0; i < this->samples; i++) {
		real[i] -= avg;
	}
}

void FFT::swap(double& a, double& b) {
	double temp = a;
	a = b;
	b = temp;
}

void FFT::compute() {
	// Reverse bits /
	uint16_t j = 0;
	for (uint16_t i = 0; i < (this->samples - 1); i++) {
		if (i < j) {
			swap(this->real[i], this->real[j]);
		}
		uint16_t k = (this->samples >> 1);
		while (k <= j) {
			j -= k;
			k >>= 1;
		}
		j += k;
	}
	
	// e^(j*PI) where wm_r is real part and wm_i is imag part
	double wm_r = -1.0;
	double wm_i = 0.0;
	
	int log2 = log(this->samples)/log(2);
	
	for(uint16_t s = 1; s <= log2; s++) {
		uint16_t m = 1 << s;
		uint16_t m2 = m >> 1;
		
		double w_r = 1.0;
		double w_i = 0.0;
		
		for(uint16_t j = 0; j < m2; j++) {
			for(uint16_t k = j; k < this->samples; k += m) {
				double t_r = (w_r*this->real[k+m2] - w_i*this->imag[k+m2]);
				double t_i = (w_r*this->imag[k+m2] + w_i*this->real[k+m2]);
				
				real[k+m2] = real[k] - t_r;
				imag[k+m2] = imag[k] - t_i;
				
				real[k] = real[k] + t_r;
				imag[k] = imag[k] + t_i;
			}
			double z = w_r*wm_r - w_i*wm_i;
			w_i = w_r*wm_i + w_i*wm_r;
			w_r = z;
		}
		
		//Calculating next complex exponential from half-angle formula
		wm_i = -sqrt((1.0 - wm_r)/2.0);
		wm_r = sqrt((1.0 + wm_r)/2.0);
	}
}

void FFT::updateSamples(double* real, double* imag) {
	this->real = real;
	this->imag = imag;
}

void FFT::getMags() {
	for(uint16_t i =0; i < this->samples; i++) {
		this->real[i] = hypot(this->real[i], this->imag[i]);
	}
}

double FFT::peak() {
	double maxY = 0;
	uint16_t maxIndex = 0;

	for (uint16_t i = 1; i < ((this->samples >> 1) + 1); i++) {
		if (this->real[i] > maxY) {
			maxY = this->real[i];
			maxIndex = i;
		}
	}
	
	return (double(maxIndex) * this->sampleFreq / double(this->samples));
}


