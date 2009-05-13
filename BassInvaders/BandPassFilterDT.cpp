/*
 * BandPassFilterDT.cpp
 *
 *  Created on: May 3, 2009
 *      Author: Darren Golbourn
 */

#include "BandPassFilterDT.h"
#include "SDL.h"

double BandPassFilterDT::alpha = 0.1;
double BandPassFilterDT::alpha0 = 1;
uint32_t BandPassFilterDT::coolDown = 2;
double BandPassFilterDT::step = 0.01;

void BandPassFilterDT::highPassFilterEffect(int chan, void *stream, int len, void *udata)
{
	static uint32_t lastTickCount = 0;
	uint32_t now = SDL_GetTicks();
	uint32_t delta = now - lastTickCount;

	if ((delta > coolDown) && fabs(alpha0 - alpha)>step )
	{
		lastTickCount = now;
		alpha0 += step * (alpha0 > alpha ? -1: 1);
	}

	((BandPassFilterDT*)udata)->high_pass((uint8_t*)stream, alpha0);
}

void BandPassFilterDT::lowPassFilterEffect(int chan, void *stream, int len, void *udata)
{
	static uint32_t lastTickCount = 0;
	uint32_t now = SDL_GetTicks();
	uint32_t delta = now - lastTickCount;

	if ((delta > coolDown) && fabs(alpha0 - alpha)>step )
	{
		lastTickCount = now;
		alpha0 += step * (alpha0 > alpha ? -1: 1);
	}

	((BandPassFilterDT*)udata)->low_pass((uint8_t*)stream, alpha0);
}

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
	Lin=(LEFT(Sin,i-1));
	Rin=(RIGHT(Sin,i-1));

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

	Lout=(int16_t)((LEFT(Sout,i-1))/alpha);
	Rout=(int16_t)((RIGHT(Sout,i-1))/alpha);
	Lin=(LEFT(Sin,i-1));
	Rin=(RIGHT(Sin,i-1));

	for(i=0; i<samples; i++)
	{
		LEFT(Sin,i) = (int16_t)(LEFT(Sout,i)/alpha);
		RIGHT(Sin,i) = (int16_t)(RIGHT(Sout,i)/alpha);
	}
}
