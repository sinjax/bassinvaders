/*
 *  ResourceBundle.cpp
 *  BassInvaders
 *
 *  Created by Sina Samangooei on 03/05/2009.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "ResourceBundle.h"
int ResourceBundle::isInit = 0;
std::map<char*,DataType> ResourceBundle::supportedTypes;
void ResourceBundle::initSupportedTypes()
{
	ResourceBundle::isInit = 1;
	ResourceBundle::supportedTypes["filename"] = SURFACE;
	ResourceBundle::supportedTypes["music"] = SOUND;
	ResourceBundle::supportedTypes["bodysprite"] = RESOURCE;
}

SDL_Surface * ResourceBundle::loadImage(char * filename)
{
	SDL_Surface* loadedImage = NULL;
	SDL_Surface* optimisedImage = NULL;
	loadedImage = SDL_LoadBMP(filename);

	if( loadedImage != NULL )
	{
		optimisedImage = SDL_DisplayFormat( loadedImage );
		SDL_FreeSurface( loadedImage );
	}

	return optimisedImage;
}
ResourceBundle::ResourceBundle(char * infoFile)
{
	
	if(!ResourceBundle::isInit)
		ResourceBundle::initSupportedTypes();
	
	FILE* fp;
	if((fp = fopen(infoFile, "r")) == NULL)
	{
		printf("Couldn't open file\n");
		return;
	}
	
	char buffer[255] = {0};
	


	fgets(buffer, 255, fp);
	while(!feof(fp))
	{
		char * key = strtok (buffer,":");
		char * value = strtok (NULL, "\n");
		
		char * pKey = new char[255];
		char * pVal = new char[255];
		strcpy(key,pKey);
		strcpy(value,pVal);
		DataType d = ResourceBundle::supportedTypes[pKey];
		switch(d)
		{
			case STRING:
				this->data[pKey] = (void*)pVal;
			break;
			case SOUND:
				this->data[pKey] = (void*)(new SoundSource(pVal));
			break;
			case SURFACE:
				this->data[pKey] = (void*)(ResourceBundle::loadImage(pVal));
			break;
			case RESOURCE:
				this->data[pKey] = (void*)(new ResourceBundle(pVal));
			break;
			default:
				// Unhandled resource datatype
			break;
				
		}
		fgets(buffer, 255, fp);
	}
}
ResourceBundle::~ResourceBundle()
{
}





