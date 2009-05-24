/*
 * BassInvaders.h
 *
 *  Created on: 19-Apr-2009
 *      Author: spijderman
 */

#ifndef BASSINVADERS_H_
#define BASSINVADERS_H_

#include <deque>
#include "Renderable.h"
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
#include "RenderableManager.h"
#include "Bullet.h"

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

	static BassInvaders * theGame;
	// End level class
	static void MusicPlayer(void *udata, Uint8 *stream, int len);

private:
	void injectState(GameStates_t newState);
	void updateStates();

private:
	void doPlayingState();

private:
	void doPausedState();
	void pauseGame();

private:
	void doEndOfLevelState();

private:
	void doLoadingState();
	void loadLevel();

private:
	bool running;// is the main game loop still running?

	Hero* pHero;
	Background* pBG;
	GameStates_t gameState;
	GameStates_t nextState;
	WindowManager wm;
	InputManager im;
	hud *pHUD;
	RenderableManager* pRM;
};

#endif /* BASSINVADERS_H_ */

