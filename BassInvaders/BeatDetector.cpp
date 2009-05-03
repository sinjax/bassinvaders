/*
 * BeatDetector.cpp
 *
 *  Created on: May 2, 2009
 *      Author: Darren Golbourn
 */

#include "BeatDetector.h"

BeatDetector::BeatDetector(uint32_t history_len, double sensitivity, uint32_t samples){
	this->samples = samples;
	this->history_len = history_len;
	this->sensitivity = sensitivity;
	H = new history<double>(history_len);
	beat = 0;
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

	beat = (average_energy * history_len) > (sensitivity * H->total);

	/* update history buffer */
	H->record(average_energy);
}

bool BeatDetector::isBeat(){
	return beat;
}
