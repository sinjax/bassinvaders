/*
 * BeatDetector.cpp
 *
 *  Created on: May 2, 2009
 *      Author: Darren Golbourn
 */

#include "BeatDetector.h"

BeatDetector::BeatDetector(uint32_t history_len, double sensitivity, uint32_t samples, uint32_t coolDown){
	this->samples = samples;
	this->history_len = history_len;
	this->sensitivity = sensitivity;
	this->coolDown = coolDown;
	lastTickCount = SDL_GetTicks();
	H = new history<double>(history_len);
	beat = false;
}

BeatDetector::~BeatDetector() {
	delete H;
}

void BeatDetector::detect(uint8_t* stream){
	/* calculate local average energy */
	double average_energy = 0;

	for(uint32_t i = 0; i<samples; i++)
	{
		double L = (double)LEFT((int16_t *) stream, i);
		double R = (double)RIGHT((int16_t *) stream, i);
		average_energy += L*L + R*R;
	}

	/* if local average is greater than S x global average then it's a beat! */
	beat = (average_energy * history_len) > (sensitivity * H->total);

	/* update history buffer */
	H->record(average_energy);
}

bool BeatDetector::isBeat(){
	uint32_t now = SDL_GetTicks();
	uint32_t delta = now - lastTickCount;

	if ((delta > coolDown) && beat)
	{
		lastTickCount = now;
		return true;
	}

	return false;
}
