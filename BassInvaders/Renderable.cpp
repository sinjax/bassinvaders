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

Renderable::~Renderable() {
	//tidy up all our sprites
	/*std::vector<Sprite>::iterator pos;

	for (pos = sprites.begin(); pos !=sprites.end(); ++pos)
	{
		pos->destroy();
	}*/
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

RenderableType_t Renderable::getType()
{
	return type;
}

uint32_t Renderable::getHealth()
{
	return health;
}

void Renderable::collide(Renderable*)
{
}

vector<Sprite>* Renderable::getSprites()
{
	return &sprites;
}
