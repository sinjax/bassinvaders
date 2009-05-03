/*
 * Background.h
 *
 *  Created on: 19-Apr-2009
 *      Author: spijderman
 *
 *  Provides layered parallax-style scrolling background
 *  To use:
 *  create background object
 *  create LayerInfo_t struct (use createLayerFromFile to load the data from a file)
 *  addLayer to push it onto the drawing stack (well, vector)
 *  call render and pass in your screen surface
 *
 *  supports:
 *  - tiled backgrounds
 *  - different scroll speeds for different layers, based on ratio of default speed
 */

#ifndef BACKGROUND_H_
#define BACKGROUND_H_

#include <SDL/SDL.h>
#include <vector>
#include "toolkit.h"

typedef struct
{
	uint32_t layerId; //unique id for each layer. allows a specific layer to be remove (JG TODO)
	SDL_Surface* pSurface;
	SDL_Rect clipBox; //how much of the pSurface should be rendered
	uint32_t numHorizontalRepeats; //if the tiled image doesn't fill the screen how many times does it need to be repeated until it does
	uint32_t numVerticalRepeats;
	uint32_t repeatPoint; //what point should the image flip to the far side of the screen and be redrawn
	uint32_t xpos; // initial x drawing position
	uint32_t ypos;
    float scrollRatio; // how much faster (>1) or slower (<1) this layer scrolls than the base rate
    uint32_t layerTickCount;
    float layerScrollAmount; //how much this layer has scrolled
} LayerInfo_t;

class Background {
public:
	Background(uint32_t scrollRate,
			   uint32_t scrollRateTicks,
			   uint32_t screenHeight,
			   uint32_t screenWidth);
	~Background();

	void addLayer(LayerInfo_t* pLayer);
	void redraw(SDL_Surface* pScreen);
	void createLayerFromFile(LayerInfo_t* pLayer, const char* filename);
	bool removeLayer(uint32_t layerId);
	bool removeTopLayer();
	bool clearBackground();
	void pause();
	void play();
	void accelerate(uint32_t scrollRate, uint32_t acceleration);

private:
	Playstate_t playstate;
	uint32_t nextLayerId; //increments on each new layer added (not decremented on deletion)
	uint32_t scrollRate; // base speed of background
	uint32_t scrollRateTicks; //how many ticks pass before we move the background by scrollRate
	uint32_t targetScrollRate; // what speed are we aiming for
	int32_t acceleration; //how quickly we want to get to the target speed
	uint32_t accelerationTicks; //how many ticks must pass before we go faster by $acceleration
	uint32_t accelerationTickCount; // how many ticks have passed since we last accelerated
	uint32_t screenHeight; // size of the game window
	uint32_t screenWidth;
	std::vector<LayerInfo_t> layers; //numLayers X surfaces, with [0] being the furthest from screen
	//jg TODO: mutex protect the layers container in case we ever go multi threaded
};

#endif /* BACKGROUND_H_ */
