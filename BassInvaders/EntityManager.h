/*
 * EntityManager.h
 *
 *  Created on: May 7, 2009
 *      Author: Darren Golbourn, spijderman
 */

#ifndef EntityMANAGER_H_
#define EntityMANAGER_H_

#include "Entity.h"
#include <deque>
#include <SDL/SDL.h>
#include "Hero.h"

class EntityManager {
public:
	EntityManager(SDL_Surface*);
	~EntityManager();

	class Hero* pHero;
	std::deque <Entity*> bullets;
	std::deque <Entity*> enemies;
	std::deque <Entity*> powerups;

	void render(); // draw all Entities
	void doCollisions(); //check for (and act on) collisions between all Entities
	void removeInactiveEntities(); // remove inactive Entities from the lists (e.g. off screen or dead)

	void setHero(Hero* pHero);
	void addBullet(Entity* pBullet);
	void addEnemy(Entity* pEnemy);
	void addPowerUp(Entity* pPowerUp);

	uint32_t enemyCount;
	uint32_t bulletCount;

private:
	SDL_Surface* pScreen; // pointer to main screen
};

#endif /* EntityMANAGER_H_ */
