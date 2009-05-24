/*
 * EntityManager.cpp
 *
 *  Created on: May 7, 2009
 *      Author: Darren Golbourn, spijderman
 */

#include "EntityManager.h"
#include "toolkit.h"

EntityManager::EntityManager(SDL_Surface* pScreen)
{
	this->pScreen = pScreen;
}

EntityManager::~EntityManager()
{

}

void EntityManager::setHero(Hero* pHero)
{
	this->pHero = pHero;
}

void EntityManager::addBullet(Entity* pBullet)
{
	bullets.push_back(pBullet);
}

void EntityManager::addEnemy(Entity* pEnemy)
{
	enemies.push_back(pEnemy);
}

void EntityManager::addPowerUp(Entity* pPowerUp)
{
	powerups.push_back(pPowerUp);
}

void EntityManager::render()
{
	/* draw all items in this order (remember that z-order is determined by when they are drawn)
	 * - enemies
	 * - bullets
	 * - hero
	 * - powerups
	 */

	//DebugPrint(("rendering %u bullets\n", bulletCount));

	std::deque<Entity*>::iterator pos;

	for (pos = enemies.begin(); pos != enemies.end(); ++pos)
	{
		(*pos)->render(this->pScreen);
	}

	pHero->render(pScreen);

	for (pos = bullets.begin(); pos != bullets.end(); ++pos)
	{
		(*pos)->render(this->pScreen);
	}

	for (pos = powerups.begin(); pos != powerups.end(); ++pos)
	{
		(*pos)->render(this->pScreen);
	}

}

void EntityManager::doCollisions()
{
	/* check collisions in the following order:
	 * bullets vs enemies, interleaved with hero vs enemies
	 * hero vs powerups
	 *
	 * JG TODO: can we make this more efficient - e.g. fewer passes?
	 */

	std::deque<Entity*>::iterator bulletsIter;
	std::deque<Entity*>::iterator enemiesIter;
	std::deque<Entity*>::iterator powerupsIter;

	/* bullets vs enemies */
	for (bulletsIter = bullets.begin(); bulletsIter != bullets.end(); ++bulletsIter)
	{
		for (enemiesIter = enemies.begin(); enemiesIter != enemies.end(); ++enemiesIter)
		{
			(*bulletsIter)->doCollision(*enemiesIter);

		}
	}

	/* hero vs enemies */
	for (enemiesIter = enemies.begin(); enemiesIter != enemies.end(); ++enemiesIter)
	{
		pHero->doCollision(*enemiesIter);
	}

	/* hero vs powerups */
	for (powerupsIter = powerups.begin(); powerupsIter != powerups.end(); ++powerupsIter)
	{
		pHero->doCollision(*powerupsIter);
	}

}

void EntityManager::removeInactiveEntities()
{
	/* go thru each list of Entities and see if they
	 * can be removed (i.e. they are dead or off screen)
	 *
	 * the slightly tortured loop syntax is so we don't erase
	 * the element to which the iterator is referring and make ++pos
	 * do undefined things (mostly crash...)
	 */
	std::deque<Entity*>::iterator pos;
	Entity * entity;
	for (pos = bullets.begin(); pos != bullets.end();)
	{
		entity = *pos;
		if (entity->canBeRemoved())
		{
			pos = bullets.erase(pos);
			delete entity;
		}
		else
		{
			++pos;
		}
	}

	for (pos = enemies.begin(); pos != enemies.end();)
	{
		entity = *pos;
		if (entity->canBeRemoved())
		{
			pos = enemies.erase(pos);
			delete entity;
			
			//DebugPrint(("deleted enemy\n"));
		}
		else
		{
			++pos;
		}
	}

	for (pos = powerups.begin(); pos != powerups.end();)
	{
		entity = *pos;
		if ((*pos)->canBeRemoved())
		{
			pos = powerups.erase(pos);
			delete *pos;
		}
		else
		{
			++pos;
		}
	}
}
