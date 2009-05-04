/*
 * Background.cpp
 *
 *  Created on: 19-Apr-2009
 *      Author: spijderman
 */

#include "Background.h"
#include <stdio.h>
#include "toolkit.h"
#include <math.h>

Background::Background(uint32_t scrollRate,
					   uint32_t scrollRateTicks,
					   uint32_t screenHeight,
					   uint32_t screenWidth)
{
	this->scrollRate = scrollRate;
	this->targetScrollRate = scrollRate;
	this->scrollRateTicks = scrollRateTicks;
	this->acceleration = 0;
	this->accelerationTicks = 100;
	this->screenHeight = screenHeight;
	this->screenWidth = screenWidth;
	nextLayerId = 0;
	playstate = PLAYING;

}

Background::~Background() {
	while (layers.size() != 0)
	{
		/* clean up surface data for each layer
		 * then delete the layer from the vector
		 */

		SDL_FreeSurface(layers.end()->pSurface);
		layers.pop_back();
	}
}

/* addLayer() pushes a new background layer on top of all the others.
 * The struct you pass in will get the layer id filled out
 * (if you want to use it...)
 * you probably want the slowest layer at the bottom of the stack
 * but if you want to have bizarre reverse parallax, there's nothing
 * stopping you doing so
 */
void Background::addLayer(LayerInfo_t *pLayer)
{
	pLayer->layerTickCount = SDL_GetTicks();
	pLayer->layerId = nextLayerId;
	layers.push_back(*pLayer);
	nextLayerId++;
}

/* delete layer which matches given id*/
bool Background::removeLayer(uint32_t layerId)
{
	bool removedLayer = false;

	std::vector<LayerInfo_t>::iterator pos;

	for (pos = layers.begin(); pos!=layers.end(); ++pos)
	{
		if (pos->layerId == layerId)
		{
			layers.erase(pos);
			removedLayer = true;
			break;
		}
	}
	return removedLayer;
}

/* pop the top layer off the rendering stack*/
bool Background::removeTopLayer()
{
	bool clearedLayer = false;
	if (layers.empty() == false)
	{
		SDL_FreeSurface(layers.end()->pSurface);
		layers.pop_back();
		clearedLayer = true;
	}
	return clearedLayer;
}

/* deletes all background layers */
bool Background::clearBackground()
{
	bool clearedAllLayers = false;
	while (layers.empty() == false)
	{
		SDL_FreeSurface(layers.end()->pSurface);
		layers.pop_back();
		if (layers.empty() == true)
		{
			clearedAllLayers = true;
		}
	}
	return clearedAllLayers;
}

/* redraw()
 * draws each layer of the background to the screen
 * in the order in which they are placed in the layers vector
 *
 * this function should get called often by the main game loop.
 * it may not actually draw anything if the playstate is paused.
 */
void Background::redraw(SDL_Surface *pScreen)
{
	if (layers.empty() == true)
	{
		// no layers to draw
		return;
	}

	uint32_t thisTickCount = SDL_GetTicks();
	uint32_t deltaTickCount;

	switch(playstate)
	{
		case PAUSED:
		{
			for (uint32_t i = 0; i< layers.size(); i++)
			{
				/* keep resetting the tick counter while we are paused
				 * this will prevent the layers from moving*/
				layers[i].layerTickCount = thisTickCount;
			}

		}break;

		case PLAYING:
		{
			/* acceleration logic
			 * if we are not at the target speed and the required duration has passed
			 * increase scroll rate by the acceleration increment
			 */
			if (scrollRate != targetScrollRate)
			{
				DebugPrint(("scrollRate %u, target %u\n", scrollRate, targetScrollRate));
				deltaTickCount = thisTickCount - accelerationTickCount;
				if (deltaTickCount >= accelerationTicks)
				{
					DebugPrint(("Accelerating by %d\n", acceleration));
					scrollRate+=acceleration;
					accelerationTickCount = thisTickCount;
				}

				if (scrollRate == targetScrollRate)
				{
					acceleration = 0;
				}
			}

			/* each layer has it's own tick counter, so its motion
			 * is worked out independently */
			for (uint32_t i = 0; i< layers.size(); i++)
			{
				/* scroll rate is worked out based on the change in
				 * time since the layer was last drawn.
				 * If the required duration has passed, move the scroll amount
				 * on by whatever the speed/ratio dictate*/
				deltaTickCount = (thisTickCount - layers[i].layerTickCount);
				if (deltaTickCount >= scrollRateTicks)
				{
					layers[i].layerScrollAmount += (float)(scrollRate*layers[i].scrollRatio);
					layers[i].layerTickCount = thisTickCount;
				}
				if ((uint32_t)layers[i].layerScrollAmount >= layers[i].repeatPoint)
				{
					layers[i].layerScrollAmount = 0;
				}

				/* draw every required repetition of the image to screen,
				 * moving horizontally as required by the scroll rate
				 *
				 * we always need to redraw in case something has moved
				 * on the surface in front of us */
				for (uint32_t h = 0; h <= layers[i].numHorizontalRepeats; h++)
				{
					for (uint32_t v = 0; v <= layers[i].numVerticalRepeats; v++)
					{
						uint32_t xdrawpos = (h*layers[i].clipBox.w) + layers[i].xpos - (uint32_t)layers[i].layerScrollAmount;
						uint32_t ydrawpos = (v*layers[i].clipBox.h) + layers[i].ypos;

						DrawToSurface(xdrawpos,
									  ydrawpos,
									  layers[i].pSurface,
									  pScreen,
									  &(layers[i].clipBox));
					}
				}
			}
		}break;
	}
}

/* takes the filename (containing valid layer data)
 * and populates the layer info struct
 *
 * this could probably do with some error handling...
 */
void Background::createLayerFromFile(LayerInfo_t* pLayer, const char* filename)
{
	uint32_t R=0;
	uint32_t G=0;
	uint32_t B=0;
	uint32_t needsColorKey=0;
	uint32_t horizontalTiling=0;
	uint32_t verticalTiling=0;
	char buffer[255] = {0};
	char bmpfile[255] = {0};

	FILE* fp;
	if((fp = fopen(filename, "r")) == NULL)
	{
		DebugPrint(("Couldn't open file\n"));
		return;
	}

	fgets(buffer, 255, fp);
	sscanf(buffer, "filename:%s", bmpfile);
	fgets(buffer, 255, fp);
	sscanf(buffer, "colorkey:(%u,%u,%u,%u)", &needsColorKey, &R, &G, &B );

	fgets(buffer, 255, fp);
	sscanf(buffer, "drawbox:(%d, %d, %u,%u)", (int32_t*)&(pLayer->clipBox.x),
											  (int32_t*)&(pLayer->clipBox.y),
											  (uint32_t*)&(pLayer->clipBox.w),
											  (uint32_t*)&(pLayer->clipBox.h));

	fgets(buffer, 255, fp);
	sscanf(buffer, "repeatpoint:(%u)", &(pLayer->repeatPoint));

	fgets(buffer, 255, fp);
	sscanf(buffer, "tiling:(%u,%u)", &horizontalTiling, &verticalTiling );

	fgets(buffer, 255, fp);
	sscanf(buffer, "drawpos:(%u,%u)", &(pLayer->xpos), &(pLayer->ypos) );

	fgets(buffer, 255, fp);
	sscanf(buffer, "scrollRatio:(%f)", &(pLayer->scrollRatio) );

	pLayer->pSurface = LoadImage(bmpfile);

	if (needsColorKey != 0)
	{
		uint32_t colorkey = SDL_MapRGB( pLayer->pSurface->format, R, G, B );
		SDL_SetColorKey( pLayer->pSurface, SDL_SRCCOLORKEY, colorkey );
	}

	if (horizontalTiling != 0)
	{
		pLayer->numHorizontalRepeats = (uint32_t) ceil((float)screenWidth/(float)pLayer->clipBox.w);
	}
	if (verticalTiling != 0)
	{
		pLayer->numVerticalRepeats = (uint32_t) ceil((float)screenHeight/(float)pLayer->clipBox.h);
	}

	DebugPrint(("loaded background from file %s\n", filename));
}

void Background::pause()
{
	playstate = PAUSED;
}

void Background::play()
{
	playstate = PLAYING;
}

/* primes an acceleration (or deceleration)
 * the background scroll speed will change $scrollRate in increments of $acceleration
 */
void Background::accelerate(uint32_t scrollRate, uint32_t acceleration)
{
	if(((this->scrollRate - scrollRate)%acceleration)!=0)
	{
		DebugPrint(("can't get to %u from %u in increments of %u!\n", scrollRate, this->scrollRate, acceleration));
		return;
	}

	if (scrollRate == targetScrollRate)
	{
		return;
	}

	this->targetScrollRate = scrollRate;
	this->acceleration = (scrollRate < this->scrollRate) ? -(acceleration) : acceleration;
	this->accelerationTickCount = SDL_GetTicks();
}
