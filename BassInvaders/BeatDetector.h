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
 *										number of 2x2byte samples per chunk);
 *
 *					Initialize an iterator:
 *						BeatIterator *BI = B.iterator(cool down time <milliseconds>);
 *
 *					Process a chunk and detect beats:
 *						B.detect(audio stream);
 *
 *					Poll the iterator to see if there is a beat:
 *						BI->isBeat();
 *
 *					A beat iterator remembers the last time it reported a beat and
 *					wont report a new one until at least coolDown ms later, thus if you
 *					want to have multiple things checking for the same beat, those things
 *					should each have their own iterator.
 */

#ifndef BEATDETECTOR_H_
#define BEATDETECTOR_H_

#include "history.h"
#include "audefnmacros.h"
#include <stdio.h>
#include <stdint.h>
#include <SDL/SDL.h>

class BeatIterator;

class BeatDetector {
private:
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
	BeatIterator* iterator(uint32_t cooldown);
};

class BeatIterator{
	friend BeatIterator* BeatDetector::iterator(uint32_t cooldown);
private:
	BeatIterator(uint32_t coolDown, BeatDetector*);
	uint32_t coolDown; // minimum number of milliseconds to wait between reporting a positive beat
	BeatDetector *b;
	uint32_t lastTickCount;
public:
	virtual ~BeatIterator(){}
	bool isBeat();
};

#endif /* BEATDETECTOR_H_ */
