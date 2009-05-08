/*
 * Hero.cpp
 *
 *  Created on: 19-Apr-2009
 *      Author: spijderman
 */

#include "Hero.h"
#include "toolkit.h"
#include <fstream>
#include <iostream>
Hero::Hero(ResourceBundle* resource)
{
	loadHeroData(resource);

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

void Hero::loadHeroData(ResourceBundle* resource)
{
	this->health = *((int*)(*resource)["health"]);
	this->attackDamage = *((int*)(*resource)["attackdamage"]);
	Sprite heroBody(((ResourceBundle**)(*resource)["bodysprite"])[0]);

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
	
	if (actions & ACTION_DIE)
	{
		std::vector<Sprite>::iterator pos;
		for (pos = sprites.begin(); pos!=sprites.end(); ++pos)
		{
			(*pos).changeState(AS_DAMAGED);
		}
	}
	
	if (actions & ACTION_LIVE)
	{
		std::vector<Sprite>::iterator pos;
		for (pos = sprites.begin(); pos!=sprites.end(); ++pos)
		{
			(*pos).changeState(AS_IDLE);
		}
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

void Hero::collide(Renderable *)
{
	std::vector<Sprite>::iterator pos;
	for (pos = sprites.begin(); pos!=sprites.end(); ++pos)
	{
		(*pos).changeState(AS_DAMAGED);
	}
}
