/*
 * baddie.cpp
 *
 *  Created on: Apr 13, 2009
 *      Author: Darren Golbourn
 */

#include "baddie.h"

baddie::baddie(game* game) :sprite::sprite(game){
	xpos = SCREEN_WIDTH;
	ypos = rand()%SCREEN_HEIGHT;
	//pAnimationStates->spriteSheet = (SDL_Surface*)(game->getResource("bmps/Explode1.bmp"));
}

baddie::~baddie() {
}

void baddie::moveSprite()
{

	yvelocity = 0;
	xvelocity = -2*SPRITE_VELOCITY;

	updateStates();

	switch(currentState)
	{
		case IDLE:
		{
			pTempState = &pAnimationStates[IDLE];
		} break;
		case DYING:
		{
			pTempState = &pAnimationStates[DYING];
		} break;
		case DEAD:
		default:
		{
			removeMe = true;/* dead sprites (or bad states) do not get rendered */
			return;
		}break;
	}

	/* figure out if the sprite is moving about */
	Uint32 now = SDL_GetTicks();
	Uint32 delta = now - lastTickCount;
	if (delta >= velocityTicks)
	{
		lastTickCount = now;
		xpos += xvelocity;
		ypos += yvelocity;
	}

	if (xpos <= pScreen->clip_rect.x)
	{
		removeMe = true;//xpos = pScreen->clip_rect.x;
	}
	if (xpos + pTempState->spriteHeight >= pScreen->clip_rect.w)
	{
		xpos = pScreen->clip_rect.w - pTempState->spriteHeight;
	}
	if (ypos <= pScreen->clip_rect.y)
	{
		ypos = pScreen->clip_rect.y;
	}
	if (ypos + pTempState->spriteWidth >= pScreen->clip_rect.h)
	{
		ypos = pScreen->clip_rect.h - pTempState->spriteWidth;
	}

}
