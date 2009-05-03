/*
 * WindowManager.cpp
 *
 *  Created on: 19-Apr-2009
 *      Author: spijderman
 */

#include "WindowManager.h"

WindowManager::WindowManager() {
	SDL_Init(SDL_INIT_VIDEO);

	pScreen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_DEPTH, SDL_HWSURFACE|SDL_DOUBLEBUF);

	SDL_WM_SetCaption( "Bass Invaders", NULL );
}

WindowManager::~WindowManager() {
	SDL_Quit();
}

void WindowManager::Flip()
{
	SDL_Flip(pScreen);
}

SDL_Surface* WindowManager::getWindowSurface()
{
	return pScreen;
}
