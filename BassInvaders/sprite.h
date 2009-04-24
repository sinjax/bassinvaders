/*
 * sprite.h
 *
 *  Created on: 07-Apr-2009
 *      Author: spijderman
 */

#ifndef SPRITE_H_
#define SPRITE_H_

#include "includes.h"

#define SPRITE_VELOCITY 2;
enum eAnimationState
{
	IDLE = 0,
	DYING = 1,
	DEAD //DEAD IS ALWAYS THE LAST STATE, AND HAS NO ANIMATION OR SURFACE ASSOCIATED WITH IT
};

typedef struct animationState_s
{
	eAnimationState state;
	Uint8 currentAnimationStep;
	Uint8 numberOfAnimationSteps;
	SDL_Surface* spriteSheet;//load this from a file
	SDL_Rect sheetStartsAt; // coordinates of first item
	Uint32 spriteWidth;
	Uint32 spriteHeight;
	Uint32 ticksPerStep;
	Uint32 lastAnimTickCount;
	eAnimationState nextState; //some states are transitional and only run once then move to the next state
} AnimationState_t;

class sprite {
public:
	sprite(game* game);
	virtual ~sprite();
	void changeState(eAnimationState state);
	void renderSprite();
	virtual void moveSprite();
	bool removeMe;
	eAnimationState currentState;
	Sint32 xpos;

protected:
	Uint8 getNextAnimationStep(AnimationState_t* pState);
	void updateStates();
	SDL_Surface* pScreen;
	game *pGame;
	AnimationState_t* pTempState;
	AnimationState_t* pAnimationStates; //one for each different animation state
	eAnimationState pendingState;
	Uint32 lastTickCount;
	Sint32 ypos;
	Sint32 xvelocity;
	Sint32 yvelocity;
	Uint32 velocityTicks; //how many ticks pass before we move by velocity
};

#endif /* SPRITE_H_ */
