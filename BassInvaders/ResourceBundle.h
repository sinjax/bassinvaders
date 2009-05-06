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
#include <fstream>
#include "SDL.h"
#include "SoundSource.h"
#include <boost/tokenizer.hpp>
#include <boost/lexical_cast.hpp>
#include <vector>
using namespace boost;
using namespace std;
typedef enum
{
	STRING, // Its just the string value, whatever that is
	RESOURCE, // Its another resource, load it
	SURFACE, // SDL_Surface
	SOUND, // A SoundSource
	INT,
	DOUBLE,
	NSECTION,
	SECTION
}  DataType;
class ResourceBundle
{
	
private:
	static int * readIntArray(string cstr);
	static float * readFloatArray(string cstr);
	static void registerResource(string, void *);
	
	static std::map<string,void*> resourceRegister;
public:
	ResourceBundle(char *);
	virtual ~ResourceBundle ();
	std::map<std::string,void*> data;
	void * operator[](const char*);
	void print();
public:	 //The static things
	static int isInit;
	static std::map<std::string,DataType> supportedTypes;
	static void initSupportedTypes();
	static SDL_Surface* loadImage(char *);
};


#endif /* end of include guard: RESOURCEBUNDLE_H_ */
