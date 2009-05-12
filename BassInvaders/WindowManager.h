/*
 * WindowManager.h
 *
 *  Created on: 19-Apr-2009
 *      Author: spijderman
 */

#ifndef WINDOWMANAGER_H_
#define WINDOWMANAGER_H_

#include <SDL/SDL.h>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define SCREEN_DEPTH 32

/* JG TODO:
 * - handle window events?
 */
class WindowManager {
public:
	WindowManager();
	~WindowManager();
	SDL_Surface* getWindowSurface();
	void Flip();

private:
	SDL_Surface* pScreen;
};

#endif /* WINDOWMANAGER_H_ */
