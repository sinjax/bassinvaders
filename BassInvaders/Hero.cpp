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
		printf("Couldn't open file %s\n", filename);
		return;
	}

	DebugPrint(("loading hero from %s\n", filename));
	loadHeroData(fp);

	xvelocity = 0;
	yvelocity = 0;
	xpos = 100;
	ypos = 100;
	type = FRIENDLY;
	currentState = RS_ACTIVE;
	pendingState = RS_ACTIVE;

	this->velocityTicks = 10;
	/* JG TODO: load in sprites:
	 * ship/damaged ship
	 * thrusters all around
	 * charge attack? */
}

Hero::~Hero() {

}

void Hero::loadHeroData(FILE* fp)
{
	char buffer[255] = {0};
	char filename[255] = {0};

	fgets(buffer, 255, fp);
	sscanf(buffer, "health:%u", &this->health);

	fgets(buffer, 255, fp);
	sscanf(buffer, "attackdamage:%u", &this->attackDamage);

	fgets(buffer, 255, fp);
	sscanf(buffer, "bodysprite:%s", filename);

	Sprite heroBody(filename);

	sprites.push_back(heroBody);

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
	doActions();

	/* set the sprite for the main body to the same position
	 * as the Hero class
	 * we'll then, (when they exist) render all the other sprites  (e.g. thrusters)
	 * in their appropriate relative positions
	 */
	sprites[BODYSPRITE].setLocation(this->xpos, this->ypos);

	/* render all the sprites in the vector*/
	std::vector<Sprite>::iterator pos;

	uint8_t i = 0;
	for (pos = sprites.begin(); pos!=sprites.end(); ++pos)
	{
		pos->renderSprite(pScreen);
		++i;
	}
}

void Hero::setActions(ACTIONMASK actions)
{
	if (actions & ACTION_MOVE_DOWN)
	{
		DebugPrint(("moving down\n"));
		yvelocity = HERO_Y_SPEED;
	}
	else if (actions & ACTION_MOVE_UP)
	{
		DebugPrint(("moving up\n"));
		yvelocity = -(HERO_Y_SPEED);
	}
	else
	{
		yvelocity = 0;
	}

	if (actions & ACTION_MOVE_LEFT)
	{
		DebugPrint(("moving left\n"));
		xvelocity = -(HERO_X_SPEED);
	}
	else if (actions & ACTION_MOVE_RIGHT)
	{
		DebugPrint(("moving right\n"));
		xvelocity = HERO_X_SPEED;
	}
	else
	{
		xvelocity = 0;
	}

	if (actions & ACTION_SHOOT)
	{
	}
}

void Hero::doActions()
{
	uint32_t now = SDL_GetTicks();
	uint32_t delta = now - lastTickCount;

	if (delta > velocityTicks)
	{
		xpos += xvelocity;
		ypos += yvelocity;
		lastTickCount = now;
	}
}
