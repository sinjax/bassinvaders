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

uint32_t Renderable::getHealth()
{
	return health;
}
