/*
 * renderableManager.cpp
 *
 *  Created on: May 7, 2009
 *      Author: Darren Golbourn
 */

#include "renderableManager.h"

renderableManager::renderableManager(SDL_Surface* pScreen) {
	this->pScreen = pScreen;
}

renderableManager::~renderableManager() {
	// TODO Auto-generated destructor stub
}

void renderableManager::render(){
	std::deque<Renderable*>::iterator i;

	for(i=theHorde.begin(); i != theHorde.end(); ++i) {
		(*i)->render(pScreen);
	}
}

void renderableManager::check_collision(){
	std::deque<Renderable*>::iterator i,j;

	for(i=theHorde.begin(); i != theHorde.end(); ++i) {
		for(j=i+1; j != theHorde.end(); ++j) {

			if (spriteCollide(*i, *j))
			{
				(*i)->collide((*j));
				(*j)->collide((*i));
			}
		}
	}
}

void renderableManager::clean_up(){
	std::deque<Renderable*>::iterator i;

	for(i=theHorde.begin(); i != theHorde.end();) {
		Renderable *bees = *i;
		if (bees->isOffScreen(pScreen->w, pScreen->h))
		{
			i = theHorde.erase(i);
			delete bees;
		}
		else i++;
	}
}

bool renderableManager::spriteCollide(Renderable* i, Renderable* j)
{
	vector<Sprite>* I = i->getSprites();
	vector<Sprite>* J = j->getSprites();
	vector<Sprite>::iterator m;
	vector<Sprite>::iterator n;

	for(m=I->begin(); m != I->end(); ++m) {
		for(n=J->begin(); n != J->end(); ++n) {
			if (isCollidingWith(&(*n), &(*m)))
			{
				return true;
			}
		}
	}

	return false;
}

bool renderableManager::isCollidingWith(Sprite* A, Sprite* B){
	vector<CollisionRect_t> X = A->getCollisionRects();
	vector<CollisionRect_t> Y = B->getCollisionRects();
	vector<CollisionRect_t>::iterator i;
	vector<CollisionRect_t>::iterator j;
	Uint32 C[4] = {B->xpos, B->ypos, A->xpos, A->ypos};

	for(i=X.begin(); i != X.end(); ++i) {
		for(j=Y.begin(); j != Y.end(); ++j) {
			if (rectIntersect(&(*i), &(*j), C))
			{
				return true;
			}
		}
	}

	return false;
}

bool renderableManager::rectIntersect(CollisionRect_t* A, CollisionRect_t* B, coords C)
{
	Sint16 x2 = C[2]+A->x;
	Sint16 y2 = C[3]+A->y;
	Uint16 h2 = A->h;
	Uint16 w2 = A->w;
	Sint32 x1 = C[0]+B->x;
	Sint16 y1 = C[1]+B->y;
	Uint16 h1 = B->h;
	Uint16 w1 = B->w;

	bool dx, dy;

	if (x2>x1){
		dx = (x2-x1)<w1;
	}else {
		dx = (x1-x2)<w2;
	}

	if (y2>y1){
		dy = (y2-y1)<h1;
	}else {
		dy = (y1-y2)<h2;
	}
	//printf("(%i, %i, %i, %i), (%i, %i, %i, %i)\n", x2,y2,h2,w2,x1,y1,h1,w1);
	if (dx&&dy){
		return true;
	}

	return false;
}
