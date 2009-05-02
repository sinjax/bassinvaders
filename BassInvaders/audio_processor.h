/*
 * audio_processor.h
 *
 *  Created on: Apr 14, 2009
 *      Author: Darren Golbourn
 *
 * Description: This library lets you split an audio stream in to frequency bands.
 */

#ifndef AUDIO_PROCESSOR_H_
#define AUDIO_PROCESSOR_H_

#include <inttypes.h>
#include "gsl/gsl_errno.h"
#include "gsl/gsl_fft_complex.h"
#include "gsl/gsl_interp.h"
#include "history.h"

// Macros for real/imaginary right/left parts of data stream
#define REAL(z,i) ((z)[2*(i)])
#define IMAG(z,i) ((z)[2*(i)+1])

#define RIGHT(z,i) ((z)[2*(i)])
#define LEFT(z,i) ((z)[2*(i)+1])

// Macros for positive/negative frequencies in frequency domain data
#define POSITIVE(i,size) (i)
#define NEGATIVE(i,size) ((size)-(i))

typedef double* fft;

class audio_processor {
	/* wavetable and workspace are used by the fft algorithm */
	gsl_fft_complex_wavetable *wavetable;
	gsl_fft_complex_workspace *workspace;
	uint32_t length; 	// length of the sound chunk in bytes
	uint32_t bands; 	// number of bands to partition the stream in to
	uint32_t samples;	// number of 16bit-per-channel stereo samples
	uint32_t freqs; 	// number of frequencies (if you take f and -f to count as one)
	uint32_t *band; 	// array which holds the frequency indexes of the band boundaries
	bool *beat;			// array of beats.  Array is bands long, contains true when a beat is detected
	uint32_t history_len; // length of history
	history<double> **H;// history arrays
	uint8_t **partitions; // partition streams
	double sensitivity; // sensitivity of beat detector
	//uint32_t *sig; // signatures to expose from music (perhaps for random seeding)

	gsl_interp_accel * faccel;
	double *f; // frequencies
	fft data;
	int ffind(double);

	double* fft_alloc(uint8_t* stream);
	//void fft_free(double* data);

	void band_window(fft data, uint8_t * stream, uint32_t bandhi, uint32_t bandlo);

	void partition(uint8_t *stream); // split input stream into "bands" output streams
	void detect_beat(); // look for a beat
public:
	void ingest(uint8_t *stream);
	audio_processor(uint32_t,uint32_t chunk_size, uint32_t num, uint32_t, double);
	bool poll_beat(uint32_t band); // returns array of beat-states
	//uint32_t poll_sig(uint32_t band);
	void band_pass(uint8_t *stream, double flo, double fhi);
	virtual ~audio_processor();
	void process(uint8_t *stream); // feed the audio processor with the audio stream
};
#endif /* AUDIO_PROCESSOR_H_ */
