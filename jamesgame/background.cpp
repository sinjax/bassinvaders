/*
 * background.cpp
 *
 *  Created on: 07-Apr-2009
 *      Author: spijderman
 */

#include "includes.h"

background::background(Uint32 width, Uint32 height)
{
	lastTickCount = SDL_GetTicks();
	scrollRate = 1;
	scrollRateTicks = 10;
	screenWidth = width;
	screenHeight = height;

	//hardcoding woo;
	pBackground = LoadImage("bmps/starfield.bmp");
	imageHeight = 256;
	imageWidth = 256;

	horizontalRepeats = (Uint32) ceil((double)screenWidth/(double)imageWidth);
	verticalRepeats = (Uint32) ceil((double)screenHeight/(double)imageHeight);
}

background::~background()
{
	SDL_FreeSurface(pBackground);
}

void background::updateBackground(SDL_Surface* pScreen)
{
	Uint32 thisTickCount = SDL_GetTicks();
	Uint32 deltaTickCount;
	Uint32 scrollAmount;

	deltaTickCount = thisTickCount - lastTickCount;
	scrollAmount = (deltaTickCount/scrollRateTicks) * scrollRate;

	if (scrollAmount >= imageWidth)
	{
		lastTickCount = thisTickCount;
		scrollAmount = 0;
	}

	for (Uint32 i = 0; i <= horizontalRepeats; i++)
	{
		for (Uint32 j = 0; j <= verticalRepeats; j++)
		{
			DrawToSurface((i*imageWidth)-scrollAmount,
							j*imageHeight,
							pBackground,
							pScreen);
		}
	}
}
