/*
 * BandPassFilterDT.h
 *
 *  Created on: May 3, 2009
 *      Author: Darren Golbourn
 *
 *	Description: This library is a discrete-time simulation of a low-pass and a high-pass
 *				 filter.  They are not very dramatic, but they are faster and cleaner than
 *				 the FFT filter.
 *
 * Usage:
 * 		Initialize the filter:
 * 			BandPassFilterDT filter(chunk size <bytes>);
 *
 * 		Do a low-pass and/or high-pass filter on the data in stream:
 * 			filter.low_pass(chunk of stream, alpha );
 * 			filter.high_pass(chunk of stream, alpha );
 *
 * 		You need a new filter object for each stream you are working with as data
 * 		from previous chunks is used in calculating the band-pass effect.
 *
 * 		alpha is a real number between 0 and 1 which define the strength of the
 * 		filtering effect.
 *
 * 		To hook a filter in to SDL_Mixer as a sound effect use:
 * 			BandPassFilterDT::alpha = 0.1; (target alpha value)
 *  		BandPassFilterDT::alpha0 = 0.1; (starting alpha value)
 * 			BandPassFilterDT::highPassFilterEffect( sound channel, stream, stream length <bytes>, pointer to filter);
 *  	 	BandPassFilterDT::lowPassFilterEffect( sound channel, stream, stream length <bytes>, pointer to filter);
 *
 */

#ifndef BANDPASSFILTERDT_H_
#define BANDPASSFILTERDT_H_

#include "inttypes.h"
#include "audefnmacros.h"
#include "history.h"
#include <stdlib.h>
#include <string.h>

class BandPassFilterDT {
	uint32_t samples;		// number of 2 x 16bit samples per chunk
	int16_t Lout, Rout, Lin, Rin; // cache
	double *Sout;
public:
	BandPassFilterDT(uint32_t);
	virtual ~BandPassFilterDT();
	void low_pass(uint8_t *stream, double alpha);	// low pass filter
	void high_pass(uint8_t *stream, double alpha); // high pass filter

	static double alpha; // can use this to hold info for SDL_Mixer
	static double alpha0;
	void static highPassFilterEffect(int chan, void *stream, int len, void *udata);
	void static lowPassFilterEffect(int chan, void *stream, int len, void *udata);
};

#endif /* BANDPASSFILTERDT_H_ */
