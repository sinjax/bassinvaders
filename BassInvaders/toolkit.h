/*
 * toolkit.h
 *
 *  Created on: 19-Apr-2009
 *      Author: spijderman
 */

#ifndef TOOLKIT_H_
#define TOOLKIT_H_

#include "SDL.h"

#ifdef DEBUG
#define DebugPrint(x) printf x
#else
#define DebugPrint(x) ((void)0)
#endif

typedef enum
{
	PLAYING,
	PAUSED
} Playstate_t;

SDL_Surface* LoadImage(const char* filename);
void DrawToSurface( uint32_t x, uint32_t y, SDL_Surface* pSource, SDL_Surface* pDest, SDL_Rect* clip = NULL);


#endif /* TOOLKIT_H_ */
