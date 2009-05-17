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

void bomb::collide(Renderable* b){
	if (b->getType() == FRIENDLY)
	{
		changeState(RS_DEAD);

		/*std::vector<Sprite>::iterator pos;
		for (pos = sprites.begin(); pos!=sprites.end(); ++pos)
		{
			(*pos).changeState(AS_DEAD);
		}*/
	}
}
