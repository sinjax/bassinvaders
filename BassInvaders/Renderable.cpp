/*
 * Renderable.cpp
 *
 *  Created on: 19-Apr-2009
 *      Author: spijderman
 */

#include "Renderable.h"

Renderable::Renderable() {
	// TODO Auto-generated constructor stub

}

Renderable::~Renderable() {
	// TODO Auto-generated destructor stub
}

void Renderable::setVelocity(int32_t xvelocity, int32_t yvelocity)
{
	this->xvelocity = xvelocity;
	this->yvelocity = yvelocity;
}

void Renderable::changeState(RenderableState_t newState)
{
	pendingState = newState;
}

RenderableState_t Renderable::getState()
{
	return currentState;
}
