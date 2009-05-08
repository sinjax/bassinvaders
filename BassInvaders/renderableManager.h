/*
 * renderableManager.h
 *
 *  Created on: May 7, 2009
 *      Author: Darren Golbourn
 */

#ifndef RENDERABLEMANAGER_H_
#define RENDERABLEMANAGER_H_
#include <deque>
#include "Renderable.h"
#include "sprite.h"
#include "SDL.h"

typedef Uint32* coords; // x1, y1, x2, y2

class renderableManager {
	SDL_Surface* pScreen;
	bool rendrableIntersect(Renderable*, Renderable*);
	bool spriteIntersect(Sprite*, Sprite*);
	bool rectIntersect(CollisionRect_t* A, CollisionRect_t* B, coords);
public:
	std::deque <Renderable*> theHorde;
	renderableManager(SDL_Surface*);
	virtual ~renderableManager();
	void render();
	void check_collision();
	void clean_up();
};

#endif /* RENDERABLEMANAGER_H_ */
