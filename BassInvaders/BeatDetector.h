/*
 * BeatDetector.h
 *
 *  Created on: May 2, 2009
 *      Author: Darren Golbourn
 *
 *  Description:  	This is a simple beat detector library:
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
 *						bool beat = BI->isBeat();
 *
 *					A beat iterator remembers the last time it reported a beat and
 *					wont report a new one until at least coolDown ms later, thus if you
 *					want to have multiple things checking for the same beat, those things
 *					should each have their own iterator.
 *
 *					If you want several things to act on beats then use a BeatBroadcaster.
 *					You can register things of type BeatListener to the broadcaster and when a beat
 *					occurs they will all have their onBeat() methods called.
 *
 *					Initialize the broadcaster:
 *						BeatBroadcaster *BB = B.broadcaster(cool down time <milliseconds>);
 *
 *					Add BeatListeners:
 *						BB->listeners.pushBack(pointer to beat listener);
 *
 *					Poll the broadcaster, if a beat is detected everyone's onBeat() is called:
 *						BB->isBeat();
 *
 *					To hook a detector in to SDL_Mixer sound functions use the process static method:
 *						BeatDetector::process( pointer to beat detector, stream, length of stream <bytes>);
 */

#ifndef BEATDETECTOR_H_
#define BEATDETECTOR_H_

#include "history.h"
#include "audefnmacros.h"
#include <stdio.h>
#include <stdint.h>
#include "SDL.h"
#include <vector>

class BeatIterator;
class BeatBroadcaster;

/*
 * Detector
 */
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
	BeatIterator* iterator(uint32_t cooldown);
	BeatBroadcaster* broadcaster(uint32_t cooldown);

	void static process( void *udata, uint8_t *stream, int len);
};

/*
 * Iterator
 */
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

/*
 * Abstract class, have your class inherit from this if you want it to be used by a BeatBroadcaster.
 */
class BeatListener{
public:
	virtual void onBeat() = 0;
	virtual ~BeatListener() = 0;
};

/*
 * Broadcaster
 */
class BeatBroadcaster{
	friend BeatBroadcaster* BeatDetector::broadcaster(uint32_t cooldown);
private:
	BeatIterator* iterator;
	BeatBroadcaster(uint32_t cooldown, BeatDetector*);
public:
	std::vector<BeatListener*> listeners;
	virtual ~BeatBroadcaster(){}
	void isBeat();
};

#endif /* BEATDETECTOR_H_ */
