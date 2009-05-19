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
void BassInvaders::MusicPlayer(void *udata, Uint8 *stream, int len)
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

	BeatDetector::process(((BassInvaders*)udata)->beat, stream, len);
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
	pRM = new RenderableManager(wm.getWindowSurface());
}

BassInvaders::~BassInvaders() {
	delete pHero;
	//delete pBG; //The background is being deleted wrongly and acting like a dick.
					// TODO Background needs reworking to use the resourceBundle anyway.
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
	/* Load the level */
	ResourceBundle *level = ResourceBundle::getResource("resources/levels/level-test.info");

	/* set up background */

	pBG = new Background(1, 10, SCREEN_HEIGHT, SCREEN_WIDTH);
	LayerInfo_t bgLayer;
	memset(&bgLayer, 0, sizeof(LayerInfo_t));
	pBG->createLayerFromFile(&bgLayer, "resources/background/b0.info");
	pBG->addLayer(&bgLayer);
	memset(&bgLayer, 0, sizeof(LayerInfo_t));
	pBG->createLayerFromFile(&bgLayer, "resources/background/b1.info");
	pBG->addLayer(&bgLayer);
	memset(&bgLayer, 0, sizeof(LayerInfo_t));
	pBG->createLayerFromFile(&bgLayer, "resources/background/b2.info");
	pBG->addLayer(&bgLayer);

	// create the hero and stuff him into the renderable manager
	pHero = new Hero(ResourceBundle::getResource("resources/hero/heroclass.info"), pRM);
	pRM->setHero(pHero);

	/*
	 * Set up the music playback, filters and beat detection environment
	 */
	// where the music comes from.
	soundSource = (SoundSource*)((*level)["music"]);

	// this is how many 2 x 2byte samples are in a chunk
	int chunkSampleLength = soundSource->spec.samples;

	// What the music is played by.
	// OpenAudio should be initialised with chunk_size = samples
	// and soundIter should be set up to take the appropriate number of bytes (i.e. samples x 4).
	Mix_OpenAudio( soundSource->spec.freq, MIX_DEFAULT_FORMAT, soundSource->spec.channels, chunkSampleLength);
	soundIter = soundSource->iter(chunkSampleLength*4);

	// fft and dt filters if you want them.
	fft = new BandPassFilterFFT (soundSource->spec.freq, chunkSampleLength*4);
	dt = new BandPassFilterDT (chunkSampleLength*4);

	// set up the beat detector.
	int historyBuffer = (int) (1.0 / ((double)(chunkSampleLength)/(double)(soundSource->spec.freq)));
	beat = new BeatDetector(historyBuffer, SENSITIVITY, chunkSampleLength );
	beatIter = beat->iterator(COOLDOWN);

	// hook the game in to the music via the MusicPlayer function.
	Mix_HookMusic(BassInvaders::MusicPlayer, this);

	/* set up the HUD */
	SDL_Color c = {55, 255, 25};
	cout << "Loading HUD with font: " << (char*)((*level)["scorefont"]) << endl;
	pHUD = new hud((char*)((*level)["scorefont"]), 20, c, wm.getWindowSurface());
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
			return;
		}

		if (event.type == SDL_KEYUP)
		{
			if ((event.key.keysym.sym == SDLK_p) && (event.key.state == SDL_RELEASED))
			{
				injectState(Paused);
			}
		}

		static int isRegistered = 0;
		if (event.type == SDL_KEYUP)
		{
			if ((event.key.keysym.sym == SDLK_a) && (event.key.state == SDL_RELEASED))
			{
				pBG->accelerate(10, 1);
				BandPassFilterDT::alpha = 0.1;

				if (!isRegistered)
				{
					Mix_RegisterEffect(MIX_CHANNEL_POST, BandPassFilterDT::highPassFilterEffect, NULL, dt);
					isRegistered = 1;
				}
			}

			if ((event.key.keysym.sym == SDLK_d) && (event.key.state == SDL_RELEASED))
			{
				pBG->accelerate(1, 1);
				BandPassFilterDT::alpha = 1.;

				if (!isRegistered)
				{
					Mix_RegisterEffect(MIX_CHANNEL_POST, BandPassFilterDT::highPassFilterEffect, NULL, dt);
					isRegistered = 1;
				}
			}

			if ((event.key.keysym.sym == SDLK_n) && (event.key.state == SDL_RELEASED))
			{
				BandPassFilterDT::alpha0 = 1;
				if (isRegistered)
				{
					Mix_UnregisterEffect(MIX_CHANNEL_POST, BandPassFilterDT::highPassFilterEffect);
					isRegistered = 0;
				}
			}
		}
	}

	/* firstly, draw the background */
	pBG->redraw(wm.getWindowSurface());

	/* move the hero about and let him shoot things*/
	pHero->setActions(im.getCurrentActions());

	//DebugPrint(("is beat?\n"));
	if (beatIter->isBeat())
	{
		//DebugPrint(("add monster\n"));
		pRM->addEnemy(new monster(rand()%SCREEN_HEIGHT));
	}


	/* do collision detection */
//	DebugPrint(("do collisions\n"));
	pRM->doCollisions();

//	DebugPrint(("render\n"));
	/* draw all the active renderables */
	pRM->render();

//	DebugPrint(("remove inactives\n"));
	/* remove the dead/off screen ones */
	pRM->removeInactiveRenderables();

	/* draw the hud/overlay */
	pHUD->displayText(10,10,"Health: %i",pHero->getHealth());
	pHUD->displayText(300,10,"Score: %i",pHero->score);

	pHUD->draw();
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

		if (event.type == SDL_KEYUP)
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

