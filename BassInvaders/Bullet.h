/*
 * Bullet.h
 *
 *  Created on: 19-Apr-2009
 *      Author: spijderman
 */

#ifndef BULLET_H_
#define BULLET_H_

#include "Entity.h"
#include "ResourceBundle.h"

#define BULLET_X_SPEED 10
#define BULLET_Y_SPEED 0

#define BULLET_RED_SPRITE_INDEX 0
/*JG TODO: for now, let's just have one type of bullet.
 * However, we will eventually want to have multiple kinds.
 * We will do this with the bullet having N sprites, one for each ammo type.
 * The type is set at instantiation time.
 */

class Bullet: public Entity {
public:
	Bullet(int32_t xpos, int32_t ypos);
	virtual ~Bullet();
	virtual bool isOffScreen(int32_t screenWidth, int32_t screenHeight);
	virtual void render(SDL_Surface *pScreen);
	virtual bool canBeRemoved();
	virtual void doCollision(Entity* pOther);
	virtual std::vector<Sprite> getActiveSpriteList();
	virtual void reactToCollision(Entity* pOther);

protected:
	virtual void updateStates();

private:
	void loadBulletData();
	uint32_t number;
};

#endif /* BULLET_H_ */
