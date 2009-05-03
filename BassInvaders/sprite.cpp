/*
 * Sprite.cpp
 *
 *  Created on: 19-Apr-2009
 *      Author: spijderman
 */

#include "Sprite.h"
#include "toolkit.h"

Sprite::Sprite(char* filename/*ResourceBundle * resources, BassInvaders * game*/) {
	/* take a text file as a parameter containing all the data for all the states
	 * then pass file into a function which populates an AnimationState_t
	  * note: in real game, will need to store checksums of all data files to confirm vanilla operation*/
		FILE* fp;
		if((fp = fopen(filename, "r")) == NULL)
		{
			printf("Couldn't open file %s\n", filename);
			return;
		}

		DebugPrint(("loading from %s\n", filename));
		loadSpriteData(fp);

		currentState = AS_IDLE;
		pendingState = AS_IDLE;
}

Sprite::~Sprite() {
	/* DON'T free up the surfaces in here.
	 * It's possible that the object was created on the stack
	 * then copied into a container class.
	 * If the owner of the container behaves nicely they will
	 * call the destroy() function when they are done with the sprite
	 */

}

void Sprite::destroy()
{
	for(uint32_t i = 0; i<AS_STATES_SIZE; ++i)
	{
		if (animationStateData[i].state != 0)
		{
			//SDL_FreeSurface(animationStateData[i].spriteSheet);
		}
	}
}

void Sprite::changeState(AnimationState_t newState)
{
	if ((currentState == newState)
		|| (pendingState == newState))
	{
		return;
	}

	if (animationStateData[newState].state == 0)
	{
		DebugPrint(("Can't transition to state with no data\n"));
		return;
	}

	switch(currentState)
	{
		/* JG TODO: do we need any further logic?*/
		case AS_IDLE:
		{
			pendingState = newState;
		}break;

		case AS_DAMAGED:
		{
			pendingState = newState;
		} break;

		case AS_DYING:
		case AS_DEAD:
		default:
		{
			//there's no return from '86...
		}break;
	}
}

void Sprite::renderSprite(SDL_Surface *pScreen)
{
	AnimationStateData_t* pTempState;

	updateStates();

	switch(currentState)
	{
		case AS_IDLE:
		{
			pTempState = &(animationStateData[AS_IDLE]);
		} break;
		case AS_DAMAGED:
		{
			pTempState = &(animationStateData[AS_DAMAGED]);
		} break;
		case AS_DYING:
		{
			pTempState = &(animationStateData[AS_DYING]);
		} break;
		case AS_DEAD:
		default:
		{
			/* dead sprites (or bad states) do not get rendered */
			return;
		}break;
	}

	if (pTempState->state == 0)
	{
		DebugPrint(("No data for this state %x\n", currentState));
		return;
	}

	//this cuts the appropriate frame out of the sprite sheet
	SDL_Rect spriteRect;
	spriteRect.x = pTempState->sheetStartsAt.x
					+ (pTempState->currentAnimationStep * pTempState->spriteWidth)
					+ pTempState->currentAnimationStep;
	spriteRect.y = pTempState->sheetStartsAt.y;
	spriteRect.w = pTempState->spriteWidth;
	spriteRect.h = pTempState->spriteHeight;

	DrawToSurface(xpos,
				  ypos,
				  pTempState->spriteSheet,
				  pScreen,
				  &spriteRect);
}

uint8_t Sprite::getNextAnimationStep(const AnimationStateData_t* pStateData)
{
	if (pStateData->currentAnimationStep == (pStateData->numberOfAnimationSteps - 1))
	{
		return 0;
	}
	else
	{
		return (pStateData->currentAnimationStep+1);
	}
}

void Sprite::updateStates()
{
	AnimationStateData_t* pCurrentState = &animationStateData[currentState];

	/* this may be the last frame in a single pass state. if so , setup the next state here*/
	if (pCurrentState->currentAnimationStep == (pCurrentState->numberOfAnimationSteps-1))
	{
		pendingState = pCurrentState->nextState;
	}

	/* change state if we are not in the one we
	 * should be in
	 */
	if (pendingState != currentState)
	{
		currentState = pendingState;

		pCurrentState = &animationStateData[currentState];
		pCurrentState->currentAnimationStep = 0;
		pCurrentState->lastAnimTickCount = SDL_GetTicks();
	}

	/* special case - dead state
	 * we do nothing if the sprite is meant to be dead*/
	if (currentState == AS_DEAD)
	{
		return;
	}

	/* figure out which animation step we are in */
	uint32_t now = SDL_GetTicks();
	uint32_t delta = now - pCurrentState->lastAnimTickCount;
	if (delta > pCurrentState->ticksPerStep)
	{
		pCurrentState->currentAnimationStep = getNextAnimationStep(pCurrentState);
		pCurrentState->lastAnimTickCount = now;
	}
}

void Sprite::loadSpriteData(FILE *fp)
{
	/*read first line, should be number of states
	 * then loop through file parsing each line until
	 * we read all the data we expected
	 * you should probably add some error handling code at some point*/
	AnimationStateData_t* pData;
	uint32_t numberOfStates;
	uint32_t R=0;
	uint32_t G=0;
	uint32_t B=0;
	uint32_t numberOfCollisionRects;
	AnimationState_t state;
	char buffer[255] = {0};
	char filename[255] = {0};


	fgets(buffer, 255, fp);
	sscanf(buffer, "numberofstates:%u", &numberOfStates);

	memset(animationStateData, 0, (sizeof(AnimationStateData_t) * AS_STATES_SIZE));

	for (uint32_t i = 0; i<numberOfStates; ++i)
	{
		fgets(buffer, 255, fp);
		sscanf(buffer, "state:%d", (int*)&state);

		pData = &(animationStateData[state]);
		pData->state = state;

			DebugPrint((" loading state 0x%x\n", state));

		fgets(buffer, 255, fp);
		sscanf(buffer, "filename:%s", filename);
			DebugPrint(("  filename %s\n", filename));

		fgets(buffer, 255, fp);
		sscanf(buffer, "colorkey:(%u,%u,%u)", &R, &G, &B );
			DebugPrint(("  colorkey:(%u,%u,%u)\n", R, G, B));

		fgets(buffer, 255, fp);
		sscanf(buffer, "nextstate:%d", (int*)&(pData->nextState));
			DebugPrint(("  nextstate:%d\n", pData->nextState));

		fgets(buffer, 255, fp);
		sscanf(buffer, "numberofanimationsteps:%u", (int*)&(pData->numberOfAnimationSteps));
			DebugPrint(("  numberofanimationsteps:%u\n", pData->numberOfAnimationSteps));

		fgets(buffer, 255, fp);
		sscanf(buffer, "ticksperstep:%u", &(pData->ticksPerStep));
			DebugPrint(("  ticksperstep:%u\n", pData->ticksPerStep));

		fgets(buffer, 255, fp);
		sscanf(buffer, "sheetstartsat:(%d,%d)", (int*)&(pData->sheetStartsAt.x), (int*)&(pData->sheetStartsAt.y) );
			DebugPrint(("  sheetstartsat:(%d,%d)\n", pData->sheetStartsAt.x, pData->sheetStartsAt.y));

		fgets(buffer, 255, fp);
		sscanf(buffer, "spritesize:(%d,%d)", &(pData->spriteWidth), &(pData->spriteHeight) );
			DebugPrint(("  spritesize:(%d,%d)\n", pData->spriteWidth, pData->spriteHeight));

		fgets(buffer, 255, fp);
		sscanf(buffer, "numberofrects:%u", (int*)&numberOfCollisionRects);
		DebugPrint(("  numberofrects:%u\n", numberOfCollisionRects));
		for (uint32_t j = 0; j<numberOfCollisionRects; ++j)
		{
			CollisionRect_t rect = {0,0,0,0};

			fgets(buffer, 255, fp);
			sscanf(buffer, "rect:(%u,%u,%d,%d)", (int*)&(rect.top),
												 (int*)&(rect.left),
												 (int*)&(rect.bottom),
												 (int*)&(rect.right));
			DebugPrint(("  rect:(%u,%u,%d,%d)\n", rect.top, rect.left, rect.bottom, rect.right));
			pData->collisionRects.push_back(rect);
		}

		pData->spriteSheet = LoadImage(filename);
		uint32_t colorkey = SDL_MapRGB( pData->spriteSheet->format, R, G, B );
		SDL_SetColorKey( pData->spriteSheet, SDL_SRCCOLORKEY, colorkey );
	}
}

void Sprite::setLocation(uint32_t xpos, uint32_t ypos)
{
	this->xpos = xpos;
	this->ypos = ypos;
}
