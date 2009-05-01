/*
 * audio_processor.cpp
 *
 *  Created on: Apr 14, 2009
 *      Author: Darren Golbourn
 */

#include "audio_processor.h"

/*
 * Initialize the stream partition library
 * Input: 	chunksize = number of bytes in the sound sample
 * 			num = number of bands to partition the sound in to
 * 			buf_sz = size of history buffer
 * 			sen = sensitivity of beat detector
 */
audio_processor::audio_processor(uint32_t chunk_size, uint32_t num, uint32_t buf_sz, double sen){
	length = chunk_size;
	bands = num;
	samples = length/4;
	freqs = samples/2;
	history_len = buf_sz;
	sensitivity = sen;
	//sig = new uint32_t[bands];
	beat = new bool[bands];
	for (uint32_t i=0; i<bands; i++) beat[i] = false;

	/* allocate fast Fourier transform stuff */
	workspace = gsl_fft_complex_workspace_alloc (samples);
	wavetable = gsl_fft_complex_wavetable_alloc (samples);

	/* allocate band partition boundaries.  The bands are logarithmically spaced
	 * i.e. band 1 width = 1
	 * 		band 2 width = 2
	 * 		band 3 width = 4
	 * 		band n width = 2^(n-1)
	 */
	uint32_t interval = freqs/(2^bands - 1); // denominator is sum of band widths SUM[2^n, n=0...N-1] = 2^N - 1
	band = new uint32_t[bands+1];

	band[0] = 1;
	for (uint32_t i = 1; i<bands; i++)
	{
		band[i] = band[i-1] + interval * 2^(i-1);
	}
	band[bands] = freqs;

	/* some stuff for the beat detection algorithm */
	H = new history<double>*[bands];
	for (uint32_t i = 0; i< bands; i++){
		H[i] = new history<double>(history_len);
	}

	partitions = new uint8_t*[bands];
	for (uint32_t i = 0; i< bands; i++){
		partitions[i] = new uint8_t[length];
	}
}

/*
 * find average energy and see if it's a beat
 */
void audio_processor::detect_beat()
{
	for (uint32_t j=0; j<bands; j++)
	{
		/* calculate local average energy */
		double average_energy = 0;

		for(uint32_t i = 0; i<samples; i++)
		{
			double L = (double)LEFT((int16_t *) partitions[j], i);
			double R = (double)RIGHT((int16_t *) partitions[j], i);
			average_energy += L*L + R*R;
		}

		beat[j] = (average_energy * history_len) > (sensitivity * H[j]->total);
		//beat[j] = (average_energy) > (H[j]->average+2*H[j]->sigma);

		/* update history buffer */
		H[j]->record(average_energy);

		/* expose a feature as a signature */
		//sig[j] = H[j]->sigma;
	}
}

/*
 * Free the allocated data types
 */
audio_processor::~audio_processor() {
	delete[] band;
	delete[] beat;
	//delete[] sig;
	for (uint32_t i = 0; i< bands; i++){
		delete H[i];
		delete partitions[i];
	}

	delete[] H;
	delete[] partitions;
	gsl_fft_complex_wavetable_free (wavetable);
	gsl_fft_complex_workspace_free (workspace);
}

bool audio_processor::poll_beat(uint32_t ba){
	return beat[ba];
}

/*uint32_t audio_processor::poll_sig(uint32_t ba){
	return sig[ba];
}*/

void audio_processor::process(uint8_t *stream){
	partition(stream);
	detect_beat();
}

/*
 * Partition the audio stream in to frequency bands
 *
 * input:	stream = pointer to the audio stream
 * 			partitions = array of pointers to the new audio streams for the frequency band partitions
 *
 */
void audio_processor::partition(uint8_t *stream){

	double data[2*samples]; // fft takes doubles not ints
	/* cast stream to int16_t then each of those to a double */
	for (uint32_t i =0; i<samples; i++){
		REAL(data,i)=(double)RIGHT((int16_t*)stream,i);
		IMAG(data,i)=(double)LEFT((int16_t*)stream,i);
	}

	/* Fourier transform */
	gsl_fft_complex_forward(data, 1, samples, wavetable, workspace);

	/* Note:
	 * freq = 0 mode is static so shouldn't affect the sound if set to 0 (it's not a "wave").
	 * This will have the effect of "centering" the time-domain wave around z = 0
	 * (i.e. graph of z will now have equal area above and below the z = 0 axis).
	 */
	 //REAL(data,0) = 0.;
	 //IMAG(data,0) = 0.;

	/*
	 * Split the data in to frequency bands. (logarithmically spaced)
	 */
	double band_data[2*samples];

	for (uint32_t j=0; j<bands; j++) // For each band...
	{
		uint8_t *band_stream = partitions[j]; // ...fill the relevant partition stream...

		for(uint32_t i=0; i<freqs; i++) // ...by going over each frequency...
		{
			if ((i < band[j+1]) && (i>=band[j])) // ...and copying over the frequencies in the the band window...
			{
				REAL(band_data,POSITIVE(i,samples)) = REAL(data,POSITIVE(i,samples));
				IMAG(band_data,POSITIVE(i,samples)) = IMAG(data,POSITIVE(i,samples));
				REAL(band_data,NEGATIVE(i,samples)) = REAL(data,NEGATIVE(i,samples));
				IMAG(band_data,NEGATIVE(i,samples)) = IMAG(data,NEGATIVE(i,samples));
			}
			else // ...and setting the other frequencies to zero.
			{
				REAL(band_data,POSITIVE(i,samples)) = 0.;
				IMAG(band_data,POSITIVE(i,samples)) = 0.;
				REAL(band_data,NEGATIVE(i,samples)) = 0.;
				IMAG(band_data,NEGATIVE(i,samples)) = 0.;
			}
		}

		gsl_fft_complex_inverse(band_data, 1, samples, wavetable, workspace); // Then reverse Fourier transform back to the time domain...

		for (uint32_t i =0; i<samples; i++){ // ...and copy the data in to the correct partition stream.
			RIGHT((int16_t*)band_stream,i)=(int16_t)REAL(band_data,i);
			LEFT((int16_t*)band_stream,i)=(int16_t)IMAG(band_data,i);
		}
	}

}
