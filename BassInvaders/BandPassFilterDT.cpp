/*
 * BandPassFilterDT.cpp
 *
 *  Created on: May 3, 2009
 *      Author: Darren Golbourn
 */

#include "BandPassFilterDT.h"

BandPassFilterDT::BandPassFilterDT(uint32_t chunk_size) {
	samples = chunk_size/4;
	L = new history<int16_t>(1);
	R = new history<int16_t>(1);
}

BandPassFilterDT::~BandPassFilterDT() {
	delete []L;
	delete []R;
}

void BandPassFilterDT::low_pass(uint8_t *stream, double alpha)
{
	int16_t* S = (int16_t*)stream;
	uint32_t i = 0;

	LEFT(S,i) = alpha * LEFT(S,i) + (1 - alpha) * L->current();
	RIGHT(S,i) = alpha * RIGHT(S,i) + (1 - alpha) * R->current();

	for(i=1; i<samples; i++)
	{
		LEFT(S,i) = alpha * LEFT(S,i) + (1 - alpha) * LEFT(S,i-1);
		RIGHT(S,i) = alpha * RIGHT(S,i) + (1 - alpha) * RIGHT(S,i-1);
	}

	L->record(LEFT(S,i));
	R->record(LEFT(S,i));
}



void BandPassFilterDT::high_pass(uint8_t *stream, double alpha)
{

}
