/*
 * BassInvaders.cpp
 *
 *  Created on: 19-Apr-2009
 *      Author: spijderman
 */

#include "BassInvaders.h"
#include "WindowManager.h"
#include "toolkit.h"

void MusicPlayer(void *udata, Uint8 *stream, int len)
{
	SoundSourceIterator* iter = ((BassInvaders*)udata)->soundIter;
	if(iter->hasNext())
	{
		SoundSample * sample = iter->next();
		for(int i = 0; i < sample->len; i++)
		{
			stream[i] = sample->sample[i];
		}
	}
	band_separate(udata, stream, len);
}
void band_separate( void *udata, uint8_t *stream, int len){
	uint8_t bandstream[len];
	BassInvaders* g = (BassInvaders*)udata;
	g->fft->ingest(stream);
	g->fft->band_pass(bandstream, 0, 400);
	g->beat->detect(bandstream);
}

BassInvaders::BassInvaders()
{
	pHero = NULL;
	pBG = NULL;
	gameState = Initialising;
	nextState = Loading;
	running = true;
}

BassInvaders::~BassInvaders() {
	if (pHero)
	{
		delete(pHero);
	}

	if (pBG)
	{
		delete(pBG);
	}
}

void BassInvaders::goGameGo()
{
	while (running)
	{
		updateStates();
		switch (gameState)
		{
			case Loading:
			{
				doLoadingState();
			}
			break;

			case Playing:
			{
				doPlayingState();
			}
			break;

			case Paused:
			{
				doPausedState();
			}break;

			default:
			{
			}
			break;
		}

		// draw whatever has been done
		if(running)
			wm.Flip();
	}
}

void BassInvaders::injectState(GameStates_t newState)
{
	nextState = newState;
}

void BassInvaders::updateStates()
{
	if (gameState != nextState)
	{
		gameState = nextState;
		switch(gameState)
		{
			case Loading:
			{
				loadLevel();
			}break;

			case Playing:
			{
				pBG->play();
			}break;

			case Paused:
			{
				pauseGame();
			}break;

			default:
			{
			}break;
		}
		DebugPrint(("Moving to state: %u\n", gameState));
	}
}

/**************************
 * Loading logic of game loop
 *************************/
void BassInvaders::doLoadingState()
{
	injectState(Playing);
}

void BassInvaders::loadLevel()
{
	pBG = new Background(1, 10, SCREEN_HEIGHT, SCREEN_WIDTH);
	LayerInfo_t bgLayer;
	memset(&bgLayer, 0, sizeof(LayerInfo_t));
	pBG->createLayerFromFile(&bgLayer, "resources/background/b0.info");
	pBG->addLayer(&bgLayer);
	memset(&bgLayer, 0, sizeof(LayerInfo_t));
	pBG->createLayerFromFile(&bgLayer, "resources/background/b1.info");
	pBG->addLayer(&bgLayer);
	pHero = new Hero();

	foo = new Sprite("resources/sprites/enemy.info");

	// The music stuff starts here
	soundSource = new SoundSource(INSERT_YOUR_SONG_PATH_HERE);
	Mix_OpenAudio( soundSource->spec.freq, MIX_DEFAULT_FORMAT, soundSource->spec.channels, soundSource->spec.samples);
	int historyBuffer = 1.0 / ((double)(soundSource->spec.samples)/(double)(soundSource->spec.freq));
	soundIter = soundSource->iter(soundSource->spec.samples*4);
	fft = new BandPassFilterFFT (soundSource->spec.freq, soundSource->spec.samples*4);
	beat = new BeatDetector(historyBuffer, SENSITIVITY, soundSource->spec.samples );
	soundIter = new SoundSourceIterator(soundSource, soundSource->spec.samples*4);
	Mix_HookMusic(MusicPlayer, this);
}
/**************************
 * Playing logic of game loop
 *************************/
void BassInvaders::doPlayingState()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT)
		{
			running = false;
		}

		if (event.type = SDL_KEYUP)
		{
			if ((event.key.keysym.sym == SDLK_p) &&
					(event.key.state == SDL_RELEASED))
			{
				injectState(Paused);
			}
		}

		if (event.type = SDL_KEYUP)
		{
			if ((event.key.keysym.sym == SDLK_a) &&
					(event.key.state == SDL_RELEASED))
			{
				pBG->accelerate(10, 1);
			}

			if ((event.key.keysym.sym == SDLK_d) &&
					(event.key.state == SDL_RELEASED))
			{
				pBG->accelerate(1, 1);
			}
		}
	}

	/* firstly, draw the background */
	pBG->redraw(wm.getWindowSurface());
}

/**************************
 * Paused logic of game loop
 *************************/

void BassInvaders::pauseGame()
{
	pBG->pause();
}

void BassInvaders::doPausedState()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT)
		{
			running = false;
		}

		if (event.type = SDL_KEYUP)
		{
			if ((event.key.keysym.sym == SDLK_p) &&
					(event.key.state == SDL_RELEASED))
			{
				injectState(Playing);
			}
		}
	}

	pBG->redraw(wm.getWindowSurface()); //it isn't really redrawing, it's updating the bg's tick counts
}
