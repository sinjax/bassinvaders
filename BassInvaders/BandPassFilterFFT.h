/*
 * BandPassFilterFFT.h
 *
 *  Created on: May 2, 2009
 *      Author: Darren Golbourn
 *
 * Description: This library lets you split an audio stream in to frequency bands.
 */

#ifndef BANDPASSFILTERFFT_H_
#define BANDPASSFILTERFFT_H_

#include <inttypes.h>
#include "gsl/gsl_errno.h"
#include "gsl/gsl_fft_complex.h"
#include "gsl/gsl_interp.h"
#include "audefnmacros.h"

typedef double* fft_t;

class BandPassFilterFFT {
	/* wavetable and workspace are used by the fft algorithm */
	gsl_fft_complex_wavetable *wavetable;
	gsl_fft_complex_workspace *workspace;
	uint32_t length; 			// length of the sound chunk in bytes
	uint32_t bands; 			// number of bands to partition the stream in to
	uint32_t samples;			// number of 16bit-per-channel stereo samples
	uint32_t freqs; 			// number of positive frequencies
	double *f; 					// array of frequencies
	gsl_interp_accel * faccel; 	// accelerator to help searching for indices of f.
	fft_t fcache;				// cache of frequency domain data
	int ffind(double);			// search function for frequency data
	double* fft_alloc(uint8_t* stream); // allocate the fourier transform
	void fft_inverse(double* band_data, uint8_t *stream);
	void band_window(double*, uint32_t bandhi, uint32_t bandlo); // the band pass filter
public:
	BandPassFilterFFT(uint32_t, uint32_t);
	virtual ~BandPassFilterFFT();
	void ingest(uint8_t *stream); // eat a stream and fourier-transform it.
	void band_pass(uint8_t *stream, double flo, double fhi); // copy band passed version ingested data to stream.
	void writef(double*, char* file); //write frequency data to file
};

#endif /* BANDPASSFILTERFFT_H_ */
