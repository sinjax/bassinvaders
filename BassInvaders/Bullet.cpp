/*
 * Bullet.cpp
 *
 *  Created on: 19-Apr-2009
 *      Author: spijderman
 */

#include "Bullet.h"
#include "WindowManager.h"
#include "toolkit.h"

uint32_t Bullet::bulletCount = 0;

Bullet::Bullet(int32_t xpos, int32_t ypos)
{
	number = bulletCount++;

	loadBulletData();

	xvelocity = BULLET_X_SPEED;
	yvelocity = BULLET_Y_SPEED;
	this->xpos = xpos;
	this->ypos = ypos;
	type = RT_FRIENDLY;
	currentState = RS_ACTIVE;
	pendingState = RS_ACTIVE;

	/* JG TODO: put these in a file */
	health = 0;
	attackDamage = 255;

	this->velocityTicks = 10;


}

Bullet::~Bullet() {
	// TODO Auto-generated destructor stub
}

void Bullet::loadBulletData()
{
	DebugPrint(("Loading bullet %u...", number));
	ResourceBundle* resource = &(*(ResourceBundle::getResource("resources/sprites/bullet.info")));
	DebugPrint(("Success\n"));
	Sprite bulletBody(resource);
	sprites.push_back(bulletBody);
}

bool Bullet::isOffScreen(int32_t screenWidth, int32_t screenHeight)
{
	return (xpos>(int32_t)screenWidth) ? true : false;
}

void Bullet::render(SDL_Surface* pScreen)
{
	updateStates();

	uint32_t now = SDL_GetTicks();
	uint32_t delta = now - lastTickCount;

	if (delta > velocityTicks)
	{
		xpos += xvelocity;
		ypos += yvelocity;
		lastTickCount = now;
	}

	/* JG TODO:
	 * happy little case-switch once we have
	 * sorted out multiple sprites
	 */

	DebugPrint(("Rendering Bullet %u...", number));
	sprites[BULLET_RED_SPRITE_INDEX].setLocation(xpos, ypos);
	sprites[BULLET_RED_SPRITE_INDEX].renderSprite(pScreen);
	DebugPrint(("success\n"));
}

void Bullet::updateStates()
{
	currentState = pendingState;

	switch(currentState)
	{
		case RS_ACTIVE:
		{
			sprites[BULLET_RED_SPRITE_INDEX].changeState(AS_IDLE);
		}break;

		case RS_DEAD:
		{
			/* the bullet has hit something and is gone
			 * there is no dying animation so just set it to dead */
			sprites[BULLET_RED_SPRITE_INDEX].changeState(AS_DEAD);
		}break;
	}
}

/* bullets can be removed if they are off screen or
 * if they have been "killed"
 */
bool Bullet::canBeRemoved()
{
	if (isOffScreen(SCREEN_WIDTH, SCREEN_HEIGHT)) //JG TODO fix this hack
	{
		return true;
	}

	if ((currentState == RS_DEAD) &&
			(sprites[BULLET_RED_SPRITE_INDEX].getAnimationState() == AS_DEAD))
	{
		return true;
	}

	return false;
}

void Bullet::doCollision(Entity* pOther)
{
	switch(pOther->getType())
	{
		case RT_ENEMY:
		{
			if (isCollidingWith(pOther))
			{
				this->reactToCollision(pOther);
				pOther->reactToCollision(this);
			}
		}break;

		case RT_POWERUP: //JG TODO: should bullets interact with powerups?
		case RT_FRIENDLY:
		case RT_NEUTRAL:
		default:
		{
			return;
		}
	}
}

std::vector<Sprite> Bullet::getActiveSpriteList()
{
	std::vector<Sprite> ret;
	ret.push_back(sprites[BULLET_RED_SPRITE_INDEX]);
	return ret;
}

void Bullet::reactToCollision(Entity* pOther)
{
	switch(pOther->getType())
	{
		case RT_ENEMY:
		{
			/* Bullet has hit an enemy.
			 * Remember that the enemy Entity will figure out what it
			 * should be doing at this point. All the bullet cares is that it
			 * has now ceased to be.
			 */
			changeState(RS_DEAD);
		}break;

		case RT_POWERUP:
		case RT_FRIENDLY:
		case RT_NEUTRAL:
		default:
		{
			return;
		}
	}
}

