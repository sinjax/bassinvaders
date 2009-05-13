/*
 * bomb.cpp
 *
 *  Created on: 12/5/09
 *      Author: Darren Golbourn
 */

#include "bomb.h"
#include "toolkit.h"
#include <fstream>
#include "WindowManager.h"
#include <iostream>

bomb::bomb(uint32_t height)
{
	loadBombData();

	xvelocity = BOMB_X_SPEED;
	yvelocity = BOMB_Y_SPEED;
	xpos = SCREEN_WIDTH;
	ypos = height;
	type = AMMO;
	currentState = RS_ACTIVE;
	pendingState = RS_ACTIVE;

	health = 0;
	attackDamage = 0;

	this->velocityTicks = 10;
}

bomb::~bomb() {
}

void bomb::loadBombData()
{
	ResourceBundle* resource = &(*(ResourceBundle::getResource("resources/sprites/bomb.info")));
	Sprite bombBody(resource);

	sprites.push_back(bombBody);

}

bool bomb::isOffScreen(uint32_t screenWidth, uint32_t screenHeight)
{
	if ( (xpos < 0) || (xpos > (int32_t)screenWidth) )
		return true;

	return false;
}

void bomb::render(SDL_Surface* pScreen)
{

	uint32_t now = SDL_GetTicks();
	uint32_t delta = now - lastTickCount;

	if (delta > velocityTicks)
	{
		xpos += xvelocity;
		ypos += yvelocity;
		lastTickCount = now;
	}

	sprites[0].setLocation(this->xpos, this->ypos);
	sprites[0].renderSprite(pScreen);
}


bool bomb::canBeRemoved()
{
	if (isOffScreen(SCREEN_WIDTH, SCREEN_HEIGHT)) //JG TODO fix this hack
	{
		return true;
	}

	if ((currentState == RS_DEAD) && (sprites[MAIN_SPRITE].getAnimationState() == AS_DEAD))
	{
		return true;
	}

	return false;
}

void bomb::doCollision(Renderable*)
{
	
}

bool bomb::isCollidingWith(Renderable*){
	return false;
}

void bomb::updateStates()
{
	currentState = pendingState;

	switch(currentState)
	{
		case RS_ACTIVE:
		{
			sprites[MAIN_SPRITE].changeState(AS_IDLE);
		}break;

		case RS_DEAD:
		{
			/* the monster has been killed, so trigger the dying animation */
			sprites[MAIN_SPRITE].changeState(AS_DYING);
		}break;
	}
}
