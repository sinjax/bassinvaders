/*
 * BandPassFilterDT.h
 *
 *  Created on: May 3, 2009
 *      Author: Darren Golbourn
 *
 *	Description: This library is a discrete-time emulation of a low-pass and a high-pass
 *				 filter.
 *
 * Usage:
 * 		Initialize the filter:
 * 			BandPassFilterDT filter(chunk size <bytes>);
 *
 * 		Do a low-pass and/or high-pass filter on the data in stream:
 * 			filter.low_pass(chunk of stream, alpha );
 * 			filter.high_pass(chunk of stream, beta );
 *
 * 		You need a new filter object for each stream you are working with as data
 * 		from previous chunks is used in calculating the band-pass effect.
 *
 * 		alpha/beta are real numbers between 0 and 1 which define the strength of the
 * 		filtering effect.
 */

#ifndef BANDPASSFILTERDT_H_
#define BANDPASSFILTERDT_H_

#include "inttypes.h"
#include "audefnmacros.h"
#include "history.h"

class BandPassFilterDT {
	uint32_t samples;		// number of 2 x 16bit samples per chunk
	history<int16_t> *L;	// cache of left speaker
	history<int16_t> *R;	// cache of right speaker

public:
	BandPassFilterDT(uint32_t);
	virtual ~BandPassFilterDT();
	void low_pass(uint8_t *stream, double alpha);	// low pass filter
	void high_pass(uint8_t *stream, double alpha); // high pass filter
};

#endif /* BANDPASSFILTERDT_H_ */
