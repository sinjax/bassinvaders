/*
 * BandPassFilterDT.cpp
 *
 *  Created on: May 3, 2009
 *      Author: Darren Golbourn
 */

#include "BandPassFilterDT.h"

BandPassFilterDT::BandPassFilterDT(uint32_t chunk_size) {
	samples = chunk_size/4;
	Lout = 0;
	Rout = 0;
	Lin = 0;
	Rin = 0;
	Sout = (double*)malloc(2*samples*sizeof(double));
	memset(Sout, 0, 2*samples*sizeof(double));
}

BandPassFilterDT::~BandPassFilterDT() {
	free(Sout);
}

void BandPassFilterDT::low_pass(uint8_t *stream, double alpha)
{
	int16_t* Sin = (int16_t*)stream;
	uint32_t i = 0;

	LEFT(Sout,i) = alpha * (double)LEFT(Sin,i) + (1. - alpha) * (double)Lout;
	RIGHT(Sout,i) = alpha * (double)RIGHT(Sin,i) + (1. - alpha) * (double)Rout;

	for(i=1; i<samples; i++)
	{
		LEFT(Sout,i) = alpha * (double)LEFT(Sin,i) + (1. - alpha) * (double)LEFT(Sout,i-1);
		RIGHT(Sout,i) = alpha * (double)RIGHT(Sin,i) + (1. - alpha) * (double)RIGHT(Sout,i-1);
	}

	Lout=(int16_t)LEFT(Sout,i-1);
	Rout=(int16_t)RIGHT(Sout,i-1);

	for(i=0; i<samples; i++)
	{
		LEFT(Sin,i) = (int16_t)LEFT(Sout,i);
		RIGHT(Sin,i) = (int16_t)RIGHT(Sout,i);
	}
}

void BandPassFilterDT::high_pass(uint8_t *stream, double alpha)
{
	int16_t* Sin = (int16_t*)stream;

	uint32_t i = 0;

	LEFT(Sout,i) = alpha * (double)( Lout + LEFT(Sin,i) - Lin);
	RIGHT(Sout,i) = alpha * (double)( Rout + RIGHT(Sin,i) - Rin);

	for(i=1; i<samples; i++)
	{
		LEFT(Sout,i) = alpha * ( LEFT(Sout,i-1) + (double)LEFT(Sin,i) - (double)LEFT(Sin,i-1));
		RIGHT(Sout,i) = alpha * ( RIGHT(Sout,i-1) + (double)RIGHT(Sin,i) - (double)RIGHT(Sin,i-1));
	}

	Lout=(int16_t)(LEFT(Sout,i-1));
	Rout=(int16_t)(RIGHT(Sout,i-1));
	Lin=(LEFT(Sin,i-1));
	Rin=(RIGHT(Sin,i-1));

	for(i=0; i<samples; i++)
	{
		LEFT(Sin,i) = (int16_t)LEFT(Sout,i);
		RIGHT(Sin,i) = (int16_t)RIGHT(Sout,i);
	}
}
