/*
 * BassInvaders.cpp
 *
 *  Created on: 19-Apr-2009
 *      Author: spijderman
 */

#include "BassInvaders.h"
#include "WindowManager.h"
#include "toolkit.h"

/*
 * This is called by SDL Music for chunkSampleSize x 4 bytes each time SDL needs it
 */
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

/*
 * This is called by music player.  The sound stream is fed to band_separate where the
 * data is analysed and beats are detected.
 */
void band_separate( void *udata, uint8_t *stream, int len){
	uint8_t bandstream[len];
	BassInvaders* g = (BassInvaders*)udata;
	g->fft->ingest(stream);
	g->fft->band_pass(bandstream, 0, 4000);
	g->beat->detect(bandstream);
	//g->fft->band_pass(stream, 300, 4000);
	//g->dt->low_pass(stream, 0.01);
}
BassInvaders * BassInvaders::theGame = 0;
BassInvaders::BassInvaders()
{
	pHero = NULL;
	pBG = NULL;
	gameState = Initialising;
	nextState = Loading;
	running = true;
	BassInvaders::theGame = this;
}

BassInvaders::~BassInvaders() {
	delete pHero;
	delete pBG;
	delete dt;
	delete fft;
	delete soundSource;
	delete beat;
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
	/* set up background */
	pBG = new Background(1, 10, SCREEN_HEIGHT, SCREEN_WIDTH);
	LayerInfo_t bgLayer;
	memset(&bgLayer, 0, sizeof(LayerInfo_t));
	pBG->createLayerFromFile(&bgLayer, "resources/background/b0.info");
	pBG->addLayer(&bgLayer);
	memset(&bgLayer, 0, sizeof(LayerInfo_t));
	pBG->createLayerFromFile(&bgLayer, "resources/background/b1.info");
	pBG->addLayer(&bgLayer);

	/* set up the renderable manager */
	rm = new renderableManager(wm.getWindowSurface());
	pHero = new Hero(ResourceBundle::getResource("resources/hero/heroclass.info")); /* make the hero */
	rm->theHorde.push_back(pHero); /* add him to the list */

	/*
	 * Set up the music playback, filters and beat detection environment
	 */
	// where the music comes from.
	soundSource = new SoundSource(INSERT_YOUR_SONG_PATH_HERE);

	// this is how many 2 x 2byte samples are in a chunk
	int chunkSampleLength = soundSource->spec.samples * 16;

	// What the music is played by.
	// OpenAudio should be initialised with chunk_size = samples
	// and soundIter should be set up to take the appropriate number of bytes (i.e. samples x 4).
	Mix_OpenAudio( soundSource->spec.freq, MIX_DEFAULT_FORMAT, soundSource->spec.channels, chunkSampleLength);
	soundIter = soundSource->iter(chunkSampleLength*4);

	// fft and dt filters if you want them.
	fft = new BandPassFilterFFT (soundSource->spec.freq, chunkSampleLength*4);
	dt = new BandPassFilterDT (chunkSampleLength*4);

	// set up the beat detector.
	int historyBuffer = 1.0 / ((double)(chunkSampleLength)/(double)(soundSource->spec.freq));
	beat = new BeatDetector(historyBuffer, SENSITIVITY, chunkSampleLength );

	// hook the game in to the music via the MusicPlayer function.
	Mix_HookMusic(MusicPlayer, this);

	/* set up the HUD */
	SDL_Color c = {55, 255, 25};
	//h = new hud("Batang.ttf", 20, c, wm.getWindowSurface());

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

		if (event.type == SDL_KEYUP)
		{
			if ((event.key.keysym.sym == SDLK_p) &&
					(event.key.state == SDL_RELEASED))
			{
				injectState(Paused);
			}
		}

		if (event.type == SDL_KEYUP)
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

	/* then the hero sprite */
	pHero->setActions(im.getCurrentActions());

	/* ... then the hordes of enemies */
	static int enemies = 0;
	if (enemies%100 == 0) // make one new monster
	{
		rm->theHorde.push_back(new monster());
	}enemies++;

	rm->clean_up();
	rm->check_collision();
	rm->render();

	/* ... then the hud/overlay */
	//h->displayText(10,10,"Health: %i0",pHero->getHealth());
	//h->draw();
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
