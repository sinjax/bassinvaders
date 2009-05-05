/*
 *  ResourceBundle.h
 *  BassInvaders
 *
 *  Created by Sina Samangooei on 03/05/2009.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef RESOURCEBUNDLE_H_
#define RESOURCEBUNDLE_H_

#include <map>
#include <string>
#include "SDL.h"
#include "SoundSource.h"

typedef enum
{
	STRING, // Its just the string value, whatever that is
	RESOURCE, // Its another resource, load it
	SURFACE, // SDL_Surface
	SOUND, // A SoundSource
	
}  DataType;
class ResourceBundle
{
public:
	ResourceBundle(char *);
	virtual ~ResourceBundle ();
	
	std::map<char*,void*> data;
public:	 //The static things
	static int isInit;
	static std::map<char*,DataType> supportedTypes;
	static void initSupportedTypes();
	static SDL_Surface* loadImage(char *);
};


#endif /* end of include guard: RESOURCEBUNDLE_H_ */
