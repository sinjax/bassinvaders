/*
 * monster.cpp
 *
 *  Created on: 8/5/09
 *      Author: Darren Golbourn
 */

#include "monster.h"
#include "toolkit.h"
#include <fstream>
#include <iostream>
monster::monster()
{
	loadMonsterData();

	xvelocity = MONSTER_X_SPEED;
	yvelocity = MONSTER_Y_SPEED;
	xpos = 100;
	ypos = 100;
	type = ENEMY;
	currentState = RS_ACTIVE;
	pendingState = RS_ACTIVE;

	health = 0;
	attackDamage = 0;

	this->velocityTicks = 10;
}

monster::~monster() {

}

void monster::loadMonsterData()
{
	ResourceBundle* resource = &(*(ResourceBundle::getResource("resources/hero/heroclass.info")));
	ResourceBundle ** setOfBalls = (ResourceBundle**)((*resource)["bodysprite"]);
	Sprite monsterBody(setOfBalls[0]);

	sprites.push_back(monsterBody);

}

bool monster::isCollidingWith(Renderable* pRenderable)
{
	return false;
}

bool monster::isOffScreen(uint32_t screenWidth, uint32_t screenHeight)
{
	if ( (xpos < 0) || (xpos > screenWidth) || (ypos < 0) || (ypos > screenHeight) )
		return true;

	return false;
}

void monster::render(SDL_Surface* pScreen)
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

void monster::collide(Renderable* b){
}