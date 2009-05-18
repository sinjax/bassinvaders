/*
 * Renderable.cpp
 *
 *  Created on: 19-Apr-2009
 *      Author: spijderman
 */

#include "Renderable.h"
#include "toolkit.h"

Renderable::Renderable() {
	// TODO Auto-generated constructor stub

}

Renderable::~Renderable()
{
	/* you'd think it would make sense to free up all the sdl surfaces that
	 * were allocated to make this Renderable here, but actually they are
	 * handled by the Resource Bundle.
	 */
}

void Renderable::setVelocity(int32_t xvelocity, int32_t yvelocity)
{
	this->xvelocity = xvelocity;
	this->yvelocity = yvelocity;
}

void Renderable::changeState(RenderableState_t newState)
{
	if (currentState != newState)
	{
		pendingState = newState;
	}
}

RenderableState_t Renderable::getState()
{
	return currentState;
}

RenderableType_t Renderable::getType()
{
	return type;
}

int32_t Renderable::getHealth()
{
	return health;
}

uint32_t Renderable::getAttackDamage()
{
	return attackDamage;
}

bool Renderable::isCollidingWith(Renderable* pOther)
{
	/* if either renderable is dead, they can't be colliding*/
	if ((pOther->getState() == RS_DEAD)
	  || (this->getState() == RS_DEAD))
	{
		return false;
	}

	/* get local active sprite list
	 * get pOther active sprite list
	 *
	 * iterate comparing all to all...
	 * return true if any collide
	 */
	std::vector<Sprite> mySprites = this->getActiveSpriteList();
	std::vector<Sprite> otherSprites = pOther->getActiveSpriteList();

	std::vector<Sprite>::iterator myPos;
	std::vector<Sprite>::iterator otherPos;

	for (myPos = mySprites.begin(); myPos != mySprites.end(); ++myPos)
	{
		for (otherPos = otherSprites.begin(); otherPos != otherSprites.end(); ++otherPos)
		{
			if (myPos->isCollidingWith(otherPos->getCollisionRects()))
			{
				return true;
			}
		}
	}

	return false;
}
