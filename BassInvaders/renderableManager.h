/*
 * RenderableManager.h
 *
 *  Created on: May 7, 2009
 *      Author: Darren Golbourn, spijderman
 */

#ifndef RENDERABLEMANAGER_H_
#define RENDERABLEMANAGER_H_

#include "Renderable.h"
#include <deque>
#include <SDL/SDL.h>
#include "Hero.h"

class RenderableManager {
public:
	RenderableManager(SDL_Surface*);
	~RenderableManager();

	class Hero* pHero;
	std::deque <Renderable*> bullets;
	std::deque <Renderable*> enemies;
	std::deque <Renderable*> powerups;

	void render(); // draw all renderables
	void doCollisions(); //check for (and act on) collisions between all renderables
	void removeInactiveRenderables(); // remove inactive renderables from the lists (e.g. off screen or dead)

	void setHero(Hero* pHero);
	void addBullet(Renderable* pBullet);
	void addEnemy(Renderable* pEnemy);
	void addPowerUp(Renderable* pPowerUp);

	uint32_t enemyCount;
	uint32_t bulletCount;

private:
	SDL_Surface* pScreen; // pointer to main screen
};

#endif /* RENDERABLEMANAGER_H_ */
