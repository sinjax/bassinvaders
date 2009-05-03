/*
 * BeatDetector.h
 *
 *  Created on: May 2, 2009
 *      Author: Darren Golbourn
 */

#ifndef BEATDETECTOR_H_
#define BEATDETECTOR_H_

#include "history.h"
#include "audefnmacros.h"
#include <stdio.h>

class BeatDetector {
	bool beat;
	uint32_t history_len; // length of history
	history<double> *H;// history
	double sensitivity; // sensitivity of beat detector
	uint32_t samples; // number of samples in stream

public:
	BeatDetector(uint32_t hislen, double sen, uint32_t samples);
	virtual ~BeatDetector();
	void detect(uint8_t* stream);
	bool isBeat();
};

#endif /* BEATDETECTOR_H_ */
