/*
 * monster.cpp
 *
 *  Created on: 8/5/09
 *      Author: Darren Golbourn
 */

#include "monster.h"
#include "toolkit.h"
#include <fstream>
#include "WindowManager.h"
#include <iostream>

int32_t monster::speed = MONSTER_X_SPEED;

monster::monster(int32_t height)
{
	loadMonsterData();

	xvelocity = speed;
	yvelocity = MONSTER_Y_SPEED;
	xpos = SCREEN_WIDTH;
	ypos = height;
	type = RT_ENEMY;
	currentState = RS_ACTIVE;
	pendingState = RS_ACTIVE;

	/* JG TODO: put these in a file */
	health = 10;
	attackDamage = 10;

	this->velocityTicks = 10;
}

monster::~monster() {
}

void monster::loadMonsterData()
{
	ResourceBundle* resource = &(*(ResourceBundle::getResource("resources/sprites/monster.info")));
	Sprite monsterBody(resource);

	sprites.push_back(monsterBody);

}

bool monster::isOffScreen(int32_t screenWidth, int32_t screenHeight)
{
	/* TODO: this should probably call into the sprite code
	 * i.e. new method sprite::isOffScreen() */
	if ( (xpos < -50) || (xpos > (int32_t)screenWidth) )
		return true;

	return false;
}

void monster::render(SDL_Surface* pScreen)
{
	updateStates();

	uint32_t now = SDL_GetTicks();
	uint32_t delta = now - lastTickCount;

	if (delta > velocityTicks)
	{
		xvelocity = speed;
		xpos += xvelocity;
		ypos += yvelocity;
		lastTickCount = now;
	}

	sprites[MAIN_SPRITE].setLocation(this->xpos, this->ypos);
	sprites[MAIN_SPRITE].renderSprite(pScreen);
}

void monster::updateStates()
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

/* monster can be removed if they are off-screen
 * or if they are dead
 */
bool monster::canBeRemoved()
{
	if (isOffScreen(SCREEN_WIDTH, SCREEN_HEIGHT)) //JG TODO fix this hack
	{
		return true;
	}

	if ((currentState == RS_DEAD) &&
			(sprites[MAIN_SPRITE].getAnimationState() == AS_DEAD))
	{
		return true;
	}

	return false;
}


void monster::doCollision(Entity* pOther)
{
	/* read type of Entity.
	 * if enemy/powerup:
 	 *  return
	 * if friendly: (i.e. bad for monsters)
	 *   find out if collision
	 *   if true, call reactToCollision on both Entities
	 */

	switch(pOther->getType())
	{
		case RT_FRIENDLY: // hero ship and bullets are "friendly"
		{
			if (isCollidingWith(pOther))
			{
				this->reactToCollision(pOther);
				pOther->reactToCollision(this);
			}
		}break;

		case RT_POWERUP:
		case RT_ENEMY:
		case RT_NEUTRAL:
		default:
		{
			return;
		}
	}
}

std::vector<Sprite> monster::getActiveSpriteList()
{
	std::vector<Sprite> ret;
	ret.push_back(sprites[MAIN_SPRITE]);
	return ret;
}

void monster::reactToCollision(Entity* pOther)
{
	switch(pOther->getType())
	{
		case RT_FRIENDLY: // hero ship and bullets are "friendly"
		{
			//monster has been hit by a hero or a bullet!
			health -= pOther->getAttackDamage();
			if (health < 0)
			{
				changeState(RS_DEAD);
			}

			DebugPrint(("Monster hit by friendly, down to %d health\n", health));
		}break;

		case RT_POWERUP:
		case RT_ENEMY:
		case RT_NEUTRAL:
		default:
		{
			return;
		}
	}
}
