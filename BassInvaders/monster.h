/*
 * monster.h
 *
 *  Created on: 8/5/09
 *      Author: Darren Golbourn
 */

#ifndef MONSTER_H_
#define MONSTER_H_

#include "Renderable.h"
#include <vector>
#include "InputManager.h"
#include "ResourceBundle.h"

#define MONSTER_X_SPEED -2
#define MONSTER_Y_SPEED 0

#define MAIN_SPRITE 0

class monster: public Renderable {
public:
	monster(uint32_t);
	virtual ~monster();

	virtual bool isOffScreen(uint32_t screenWidth, uint32_t screenHeight);
	virtual void render(SDL_Surface *pScreen);
	virtual bool canBeRemoved();
	virtual void doCollision(Renderable* pOther);
	virtual std::vector<Sprite> getActiveSpriteList();
	virtual void reactToCollision(Renderable* pOther);

protected:
	virtual void updateStates();

private:
	void loadMonsterData();
};

#endif /* HERO_H_ */
