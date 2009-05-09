/*
 * BeatDetector.h
 *
 *  Created on: May 2, 2009
 *      Author: Darren Golbourn
 *
 *  Description:  	This is a simple beat detector.
 *  				It takes a stereo audio stream and calculates whether the
 *  				current volume is louder than the recent average volume,
 *  				If the current volume is sufficiently louder - it's a beat!
 *
 *  Usage:			Initialize the detector:
 *						BeatDetector B( number of audio chunks to hold in memory,
 *										sensitivity of detector <from 1 to 2 works best>,
 *										number of 2x2byte samples per chunk,
 *										cool down time <milliseconds> );
 *
 *					Process a chunk and detect beats:
 *						B.detect(audio stream);
 *
 *					Poll the detector to see if there is a beat currently:
 *						B.isBeat();
 */

#ifndef BEATDETECTOR_H_
#define BEATDETECTOR_H_

#include "history.h"
#include "audefnmacros.h"
#include <stdio.h>
#include <stdint.h>
#include <SDL/SDL.h>

class BeatDetector {
	bool beat;
	uint32_t history_len; // length of history
	history<double> *H;// history
	double sensitivity; // sensitivity of beat detector
	uint32_t samples; // number of samples in stream
	uint32_t coolDown; // number of milliseconds to wait between reporting a positive beat

	uint32_t lastTickCount;
public:
	BeatDetector(uint32_t hislen, double sen, uint32_t samples, uint32_t coolDown);
	virtual ~BeatDetector();
	void detect(uint8_t* stream);
	bool isBeat();
};

#endif /* BEATDETECTOR_H_ */
