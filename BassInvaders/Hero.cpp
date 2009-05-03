/*
 * Hero.cpp
 *
 *  Created on: 19-Apr-2009
 *      Author: spijderman
 */

#include "Hero.h"

Hero::Hero()
{
	/* JG TODO: load in sprites:
	 * ship/damaged ship
	 * thrusters all around
	 * charge attack? */
	health = 100;
	attackDamage = 255;
}

Hero::~Hero() {

}

bool Hero::isCollidingWith(Renderable* pRenderable)
{
	return false;
}

bool Hero::isOffScreen(uint32_t screenWidth, uint32_t screenHeight)
{
	return false;
}

void Hero::render(SDL_Surface* pScreen)
{
}
