/*
 * BandPassFilterDT.h
 *
 *  Created on: May 3, 2009
 *      Author: Darren Golbourn
 */

#ifndef BANDPASSFILTERDT_H_
#define BANDPASSFILTERDT_H_

#include "inttypes.h"
#include "audefnmacros.h"
#include "history.h"

class BandPassFilterDT {
	uint32_t samples;
	history<int16_t> *L;
	history<int16_t> *R;

public:
	BandPassFilterDT(uint32_t);
	virtual ~BandPassFilterDT();
	void low_pass(uint8_t *stream, double alpha);
	void high_pass(uint8_t *stream, double alpha);
};

#endif /* BANDPASSFILTERDT_H_ */
