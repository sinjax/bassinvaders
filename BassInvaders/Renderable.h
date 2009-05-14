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
	RT_FRIENDLY,
	RT_ENEMY,
	RT_NEUTRAL,
	RT_AMMO,
	RT_POWERUP
} RenderableType_t;

class Renderable {
public:
	Renderable();
	virtual ~Renderable();

	virtual bool isOffScreen(uint32_t screenWidth, uint32_t screenHeight) = 0;
	virtual void render(SDL_Surface *pScreen) = 0;
	void changeState(RenderableState_t newState);
	RenderableState_t getState();
	RenderableType_t getType();
	virtual void setVelocity(int32_t xvelocity, int32_t yvelocity);
	uint32_t getAttackDamage();
	int32_t getHealth();
	virtual bool canBeRemoved() = 0; //the renderable is either dead or off screen and can be removed permanently from the game
	virtual void doCollision(Renderable* pOther)=0; //what to do when it collides with another renderable
	virtual std::vector<Sprite> getActiveSpriteList() = 0; //which sprites are currently active in terms of collision detection
	virtual void reactToCollision(Renderable* pOther) = 0; // what this Renderable does when it collides with another Renderable
	// NOTE: reactToCollision must only be called ONCE for every colliding renderable pair. Otherwise they will react twice to a collision
	// It's up to the renderable manager to make sure this is not done

protected:
	virtual void updateStates() = 0;
	virtual bool isCollidingWith(Renderable* pOther);//this is only for the class to use internally. others should call doCollision and let the class figure out the effects

protected:
	std::vector<Sprite> sprites;
	int32_t xvelocity;
	int32_t yvelocity;
	RenderableType_t type;
	RenderableState_t currentState;
	RenderableState_t pendingState;
	uint32_t attackDamage;
	int32_t health;
	int32_t xpos; //this x and y may or may not bear any resemblance to the x and y pos of the encapsulated sprites
	int32_t ypos;
};

#endif /* RENDERABLE_H_ */
