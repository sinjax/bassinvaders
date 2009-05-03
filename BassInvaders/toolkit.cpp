/*
 * toolkit.cpp
 *
 *  Created on: 19-Apr-2009
 *      Author: spijderman
 */

#include "toolkit.h"

SDL_Surface* LoadImage(const char* filename)
{
	SDL_Surface* loadedImage = NULL;
	SDL_Surface* optimisedImage = NULL;
	loadedImage = SDL_LoadBMP(filename);

	if( loadedImage != NULL )
	{
		optimisedImage = SDL_DisplayFormat( loadedImage );
		SDL_FreeSurface( loadedImage );
	}

	return optimisedImage;

	DebugPrint(("Loaded image from %s\n", filename));
}

void DrawToSurface( uint32_t x, uint32_t y, SDL_Surface* pSource, SDL_Surface* pDest, SDL_Rect* clip)
{
	SDL_Rect offset;
	offset.x = x;
	offset.y = y;
	SDL_BlitSurface( pSource, clip, pDest, &offset );
}
