/*
 * sprite.cpp
 *
 *  Created on: 07-Apr-2009
 *      Author: spijderman, Darren Golbourn
 */

#include "includes.h"

sprite::sprite(game* game) {
	AnimationState_t* pTemp;
	Uint32 colorkey;
	pGame = game;
	pAnimationStates = new AnimationState_t[2];
	pScreen = game->getWindowSurface();
	pTemp = pAnimationStates;

	pTemp->state = IDLE;
	pTemp->nextState = IDLE;
	pTemp->currentAnimationStep = 0;
	pTemp->numberOfAnimationSteps = 3;
	pTemp->sheetStartsAt.x = 1;
	pTemp->sheetStartsAt.y = 1;
	pTemp->spriteWidth = 23;
	pTemp->spriteHeight = 23;
	pTemp->ticksPerStep = 1000;
	pTemp->lastAnimTickCount = SDL_GetTicks();
	pTemp->spriteSheet = (SDL_Surface*)(game->getResource("bmps/ship3.bmp"));
	colorkey = SDL_MapRGB( pTemp->spriteSheet->format, 0, 0xFF, 0 );
	SDL_SetColorKey( pTemp->spriteSheet, SDL_SRCCOLORKEY, colorkey );

	pTemp++; // do the dying state

	pTemp->state = DYING;
	pTemp->nextState = DEAD;
	pTemp->currentAnimationStep = 0;
	pTemp->numberOfAnimationSteps = 8;
	pTemp->sheetStartsAt.x = 1;
	pTemp->sheetStartsAt.y = 1;
	pTemp->spriteWidth = 23;
	pTemp->spriteHeight = 23;
	pTemp->ticksPerStep = 100;
	pTemp->lastAnimTickCount = SDL_GetTicks();
	pTemp->spriteSheet = (SDL_Surface*)(game->getResource("bmps/Explode1.bmp"));
	colorkey = SDL_MapRGB( pTemp->spriteSheet->format, 0, 0xFF, 0 );
	SDL_SetColorKey( pTemp->spriteSheet, SDL_SRCCOLORKEY, colorkey );

	currentState = IDLE;
	pendingState = IDLE;
	lastTickCount = SDL_GetTicks();
	xpos = 100;
	ypos = 100;
	velocityTicks = 10;
	xvelocity = 0;
	yvelocity = 0;

	removeMe = false;
}

sprite::~sprite() {
	delete[] pAnimationStates;
}

void sprite::moveSprite()
{
	Uint8* keys = pGame->keys;

	if (keys[SDLK_x])
	{
		changeState(DYING);
	}
	else if (keys[SDLK_r])
	{
		changeState(IDLE);
	}

	if(keys[SDLK_UP])
	{
		yvelocity = -SPRITE_VELOCITY;
	}
	else if(keys[SDLK_DOWN])
	{
		yvelocity = SPRITE_VELOCITY;
	}
	else
	{
		yvelocity = 0;
	}

	if(keys[SDLK_LEFT])
	{
		xvelocity = -SPRITE_VELOCITY;
	}
	else if(keys[SDLK_RIGHT])
	{
		xvelocity = SPRITE_VELOCITY;
	}
	else
	{
		xvelocity = 0;
	}

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
			/* dead sprites (or bad states) do not get rendered */
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

	// check for collision with edge of play area
	if (xpos <= pScreen->clip_rect.x)
	{
		xpos = pScreen->clip_rect.x;
	}
	if (xpos + pTempState->spriteHeight >= pScreen->clip_rect.w)
	{
		xpos = pScreen->clip_rect.w - pTempState->spriteHeight;
	}
	if (ypos <= pScreen->clip_rect.y)
	{
		changeState(DYING);
		ypos = pScreen->clip_rect.y;
	}
	if (ypos + pTempState->spriteWidth >= pScreen->clip_rect.h)
	{
		changeState(DYING);
		ypos = pScreen->clip_rect.h - pTempState->spriteWidth;
	}

	//check for collision with baddie sprites
	std::list<sprite*>::iterator i;
	i=pGame->sprite_list.begin();++i;
	for(; i != pGame->sprite_list.end(); ++i) {
		sprite *bees = *i;
		Uint32 x2 = bees->xpos;
		Uint32 y2 = bees->ypos;
		Uint32 a2 = 23;//bees->pTempState->spriteHeight;
		Uint32 b2 = 23;//bees->pTempState->spriteWidth;
		Uint32 x1 = xpos;
		Uint32 y1 = ypos;
		Uint32 a1 = pTempState->spriteHeight;
		Uint32 b1 = pTempState->spriteWidth;
		bool dx, dy;

		if (x2>x1){
			dx = (x2-x1)<a1;
		}else {
			dx = (x1-x2)<a2;
		}

		if (y2>y1){
			dy = (y2-y1)<b1;
		}else {
			dy = (y1-y2)<b2;
		}

		if (dx&&dy){
			changeState(DYING);
			bees->changeState(DYING);
		}
	}
}

void sprite::renderSprite()
{
	/* copy correct sprite from sheet */
	SDL_Rect spriteRect;
	spriteRect.x = pTempState->sheetStartsAt.x + (pTempState->currentAnimationStep * pTempState->spriteWidth) + pTempState->currentAnimationStep;
	spriteRect.y = pTempState->sheetStartsAt.y;
	spriteRect.w = pTempState->spriteWidth;
	spriteRect.h = pTempState->spriteHeight;

	/* and finally draw it to the screen */
	DrawToSurface(xpos,
					ypos,
					pTempState->spriteSheet,
					pScreen,
					&spriteRect);
}

Uint8 sprite::getNextAnimationStep(AnimationState_t* pState)
{
	if (pState->currentAnimationStep == (pState->numberOfAnimationSteps - 1))
	{
		return 0;
	}
	else
	{
		return (pState->currentAnimationStep+1);
	}
}

void sprite::changeState(eAnimationState state)
{
	pendingState = state;
}

void sprite::updateStates()
{
	AnimationState_t* pCurrentState = &pAnimationStates[currentState];

	/* this may be the last frame in a single pass state. if so , setup the next state here*/
	if (pCurrentState->currentAnimationStep == (pCurrentState->numberOfAnimationSteps-1))
	{
		pendingState = pCurrentState->nextState;
	}

	if (pendingState != currentState)
	{
		currentState = pendingState;

		/* special case - dead state*/
		if (currentState == DEAD)
		{
			return;
		}

		pCurrentState = &pAnimationStates[currentState];
		pCurrentState->currentAnimationStep = 0;
		pCurrentState->lastAnimTickCount = SDL_GetTicks();
	}

	/* figure out which animation step we are in */
	Uint32 now = SDL_GetTicks();
	Uint32 delta = now - pCurrentState->lastAnimTickCount;
	if (delta > pCurrentState->ticksPerStep)
	{
		pCurrentState->currentAnimationStep = getNextAnimationStep(pCurrentState);
		pCurrentState->lastAnimTickCount = now;
	}
}
