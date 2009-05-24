/*
 * Entity.h
 *
 *  Created on: 19-Apr-2009
 *      Author: spijderman
 */

#ifndef Entity_H_
#define Entity_H_

#include "Sprite.h"
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
} Entitiestate_t;

typedef enum
{
	RT_FRIENDLY,
	RT_ENEMY,
	RT_NEUTRAL,
	RT_POWERUP
} EntityType_t;

class Entity {
public:
	Entity();
	virtual ~Entity();
	virtual bool isOffScreen(int32_t screenWidth, int32_t screenHeight) = 0;
	virtual void render(SDL_Surface *pScreen) = 0;
	void changeState(Entitiestate_t newState);
	Entitiestate_t getState();
	EntityType_t getType();
	virtual void setVelocity(int32_t xvelocity, int32_t yvelocity);
	uint32_t getAttackDamage();
	int32_t getHealth();
	virtual bool canBeRemoved() = 0; //the Entity is either dead or off screen and can be removed permanently from the game
	virtual void doCollision(Entity* pOther)=0; //what to do when it collides with another Entity
	virtual std::vector<Sprite> getActiveSpriteList() = 0; //which sprites are currently active in terms of collision detection
	virtual void reactToCollision(Entity* pOther) = 0; // what this Entity does when it collides with another Entity
	// NOTE: reactToCollision must only be called ONCE for every colliding Entity pair. Otherwise they will react twice to a collision
	// It's up to the Entity manager to make sure this is not done

protected:
	virtual void updateStates() = 0;
	virtual bool isCollidingWith(Entity* pOther);//this is only for the class to use internally. others should call doCollision and let the class figure out the effects

protected:
	std::vector<Sprite> sprites;
	int32_t xvelocity;
	int32_t yvelocity;
	EntityType_t type;
	Entitiestate_t currentState;
	Entitiestate_t pendingState;
	uint32_t attackDamage;
	int32_t health;
	int32_t xpos; //this x and y may or may not bear any resemblance to the x and y pos of the encapsulated sprites
	int32_t ypos;
	uint32_t velocityTicks; //how many ticks pass before we move by velocity
	uint32_t lastTickCount;
};

#endif /* Entity_H_ */
