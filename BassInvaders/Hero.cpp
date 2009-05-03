/*
 * Hero.cpp
 *
 *  Created on: 19-Apr-2009
 *      Author: spijderman
 */

#include "Hero.h"
#include "toolkit.h"

Hero::Hero(const char* filename)
{
	FILE* fp;
	if((fp = fopen(filename, "r")) == NULL)
	{
		printf("Couldn't open file\n");
		return;
	}

	DebugPrint(("loading hero from %s:\n", filename));
	loadHeroData(fp);

	/* JG TODO: load in sprites:
	 * ship/damaged ship
	 * thrusters all around
	 * charge attack? */
}

Hero::~Hero() {

}

void Hero::loadHeroData(FILE* fp)
{
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
