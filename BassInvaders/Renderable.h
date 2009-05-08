/*
 * Renderable.h
 *
 *  Created on: 19-Apr-2009
 *      Author: spijderman
 */

#ifndef RENDERABLE_H_
#define RENDERABLE_H_

#include "sprite.h"
#include <vector>
/*JG TODO:
 * - baseclass for all on-screen things
 * - x/y pos
 * - velocity info
 * - life state machine
 * - rendering method
 * - collision detection method
 * - 0-n sprites
 * - type of object (good/bad/bullet/neutral)
 */

typedef enum
{
	RS_ACTIVE,
	RS_DEAD,
} RenderableState_t;

typedef enum
{
	FRIENDLY,
	ENEMY,
	NEUTRAL,
	AMMO
} RenderableType_t;

class Renderable {
public:
	Renderable();
	virtual ~Renderable();

	//bool isCollidingWith(Renderable* pRenderable) = 0;
	virtual bool isOffScreen(uint32_t screenWidth, uint32_t screenHeight) = 0;
	virtual void render(SDL_Surface *pScreen) = 0;
	virtual void changeState(RenderableState_t newState);
	RenderableState_t getState();
	RenderableType_t getType();
	virtual void setVelocity(int32_t xvelocity, int32_t yvelocity);
	uint32_t getAttackDamage();
	uint32_t getHealth();
	virtual void collide(Renderable*) = 0;
	vector<Sprite>* getSprites();

protected:
	std::vector<Sprite> sprites;
	int32_t xvelocity;
	int32_t yvelocity;
	RenderableType_t type;
	RenderableState_t currentState;
	RenderableState_t pendingState;
	uint32_t attackDamage;
	uint32_t health;
	int32_t xpos; //this x and y may or may not bear any resemblance to the x and y pos of the encapsulated sprites
	int32_t ypos;
};

#endif /* RENDERABLE_H_ */
