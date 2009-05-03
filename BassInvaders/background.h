/*
 * background.h
 *
 *  Created on: 07-Apr-2009
 *      Author: spijderman
 */

#ifndef BACKGROUND_H_
#define BACKGROUND_H_

#include "includes.h"

class background {
public:
	background(Uint32 width, Uint32 height);
	~background();
	void updateBackground(SDL_Surface *pScreen);

private:
	SDL_Surface* pBackground;
	Uint32 lastTickCount;
	Uint32 scrollRate;
	Uint32 scrollRateTicks; //how many ticks pass before we move the background by scrollRate
	Uint32 scrollOffset;
	Uint32 screenHeight;
	Uint32 screenWidth;
	Uint32 imageHeight;
	Uint32 imageWidth;
	Uint32 horizontalRepeats;
	Uint32 verticalRepeats;
};

#endif /* BACKGROUND_H_ */
