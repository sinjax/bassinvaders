/*
 * BassInvaders.h
 *
 *  Created on: 19-Apr-2009
 *      Author: spijderman
 */

#ifndef BASSINVADERS_H_
#define BASSINVADERS_H_

#include <deque>
#include "Entity.h"
#include "Hero.h"
#include "Background.h"
#include "WindowManager.h"
#include "InputManager.h"
#include "soundSource.h"
#include <SDL/SDL_mixer.h>
#include "BeatDetector.h"
#include "BandPassFilterFFT.h"
#include "ResourceBundle.h"
#include "BandPassFilterDT.h"
#include "monster.h"
#include "hud.h"
#include "EntityManager.h"
#include "Bullet.h"
#include "Formation.h"
#include "Scene.h"

#define SENSITIVITY 1.3
#define INSERT_YOUR_SONG_PATH_HERE "test.mp3"
#define COOLDOWN 200

/*JG TODO:
 * - game state machine
 * - game loop
 * - gets events from file/music core
 */

typedef enum
{
	Initialising,
	Loading,
	Playing,
	Paused,
	EndOfLevel
} GameStates_t;

class BassInvaders {
public:
	BassInvaders();
	~BassInvaders();

	void goGameGo(); // main game loop

	// These are called by SDL and need to be publically accessible
	// Write getters/setters if you are so inclined :-)
	// This is the level class
	SoundSource * soundSource;
	SoundSourceIterator * soundIter;
	BandPassFilterFFT *fft;
	BandPassFilterDT *dt;
	BeatDetector *beat;
	BeatIterator *beatIter;
	bool running;// is the main game loop still running?

	Hero* pHero;
	Background* pBG;
	WindowManager wm;
	InputManager im;
	hud *pHUD;
	EntityManager* pRM;
	static BassInvaders * theGame;
	// End level class
	static void MusicPlayer(void *udata, Uint8 *stream, int len);

private:
	void injectState(GameStates_t newState);
	void updateStates();
	void doPlayingState();
	void doPausedState();
	void pauseGame();
	void doEndOfLevelState();
	void doLoadingState();
	void loadLevel();

	GameStates_t gameState;
	GameStates_t nextState;
};

#endif /* BASSINVADERS_H_ */

