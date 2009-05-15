/*
 * RenderableManager.cpp
 *
 *  Created on: May 7, 2009
 *      Author: Darren Golbourn, spijderman
 */

#include "RenderableManager.h"
#include "toolkit.h"

RenderableManager::RenderableManager(SDL_Surface* pScreen)
{
	this->pScreen = pScreen;
	enemyCount = 0;
}

RenderableManager::~RenderableManager()
{

}

void RenderableManager::setHero(Hero* pHero)
{
	this->pHero = pHero;
}

void RenderableManager::addBullet(Renderable* pBullet)
{
	bullets.push_back(pBullet);
}

void RenderableManager::addEnemy(Renderable* pEnemy)
{
	enemies.push_back(pEnemy);
	enemyCount++;
}

void RenderableManager::addPowerUp(Renderable* pPowerUp)
{
	powerups.push_back(pPowerUp);
}

void RenderableManager::render()
{
	/* draw all items in this order (remember that z-order is determined by when they are drawn)
	 * - enemies
	 * - bullets
	 * - hero
	 * - powerups
	 */

	DebugPrint(("rendering %u enemies\n", enemyCount));

	std::deque<Renderable*>::iterator pos;

	for (pos = enemies.begin(); pos != enemies.end(); ++pos)
	{
		(*pos)->render(this->pScreen);
	}

	for (pos = bullets.begin(); pos != bullets.end(); ++pos)
	{
		(*pos)->render(this->pScreen);
	}

	pHero->render(pScreen);

	for (pos = powerups.begin(); pos != powerups.end(); ++pos)
	{
		(*pos)->render(this->pScreen);
	}

}

void RenderableManager::doCollisions()
{
	/* check collisions in the following order:
	 * bullets vs enemies, interleaved with hero vs enemies
	 * hero vs powerups
	 *
	 * JG TODO: can we make this more efficient - e.g. fewer passes?
	 */

	std::deque<Renderable*>::iterator bulletsIter;
	std::deque<Renderable*>::iterator enemiesIter;
	std::deque<Renderable*>::iterator powerupsIter;

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

void RenderableManager::removeInactiveRenderables()
{
	/* go thru each list of renderables and see if they
	 * can be removed (i.e. they are dead or off screen)
	 *
	 * the slightly tortured loop syntax is so we don't erase
	 * the element to which the iterator is referring and make ++pos
	 * do undefined things (mostly crash...)
	 */
	std::deque<Renderable*>::iterator pos;

	for (pos = bullets.begin(); pos != bullets.end();)
	{
		if ((*pos)->canBeRemoved())
		{
			delete(*pos);
			pos = bullets.erase(pos);
		}
		else
		{
			++pos;
		}
	}

	for (pos = enemies.begin(); pos != enemies.end();)
	{
		if ((*pos)->canBeRemoved())
		{
			delete(*pos);
			pos = enemies.erase(pos);
			DebugPrint(("deleted enemy\n"));
			enemyCount--;
		}
		else
		{
			++pos;
		}
	}

	for (pos = powerups.begin(); pos != powerups.end();)
	{
		if ((*pos)->canBeRemoved())
		{
			delete(*pos);
			pos = powerups.erase(pos);
		}
		else
		{
			++pos;
		}
	}
}
