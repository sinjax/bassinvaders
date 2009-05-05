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
#define SENSITIVITY 1.3
#define INSERT_YOUR_SONG_PATH_HERE "test.mp3"
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
	BeatDetector *beat;
	// End level class

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
	void loadLevel(); /* JG TODO: load up background, hero, music etc... */

private:
	bool running;// is the main game loop still running?

	std::deque<Renderable> baddies;
	/* Baddies contains all the bad-guy renderables
	 * this deque will get added to/deleted from when goons appear on the screen
	 *
	 * I think double ended queue is the most efficient template to use
	 * as I expect that bad guys will get killed off generally in the
	 * order in which they appear, hence having them at the front of the queue.
	 * And if they are not shot, then they'll get popped off by the engine
	 * when they drop off the edge of the screen.
	 */
	Hero* pHero;
	Background* pBG;
	GameStates_t gameState;
	GameStates_t nextState;
	WindowManager wm;
	InputManager im;


	SDL_Surface* pPauseGameScreen; /* when the pause state is invoked,
									* the current playing screen is grabbed
									* and then drawn as the first thing in the background
									*/

	Sprite* foo;
};

void MusicPlayer(void *udata, Uint8 *stream, int len);
void band_separate( void *udata, uint8_t *stream, int len);
#endif /* BASSINVADERS_H_ */

