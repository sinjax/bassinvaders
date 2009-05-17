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
#include <SDL/SDL.h>
#include "soundSource.h"
#include <boost/tokenizer.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/shared_ptr.hpp>
#include <vector>
using namespace boost;
using namespace std;

/*
Now, you might notice i've used the word parse here. This is a hack, a horrible hack. Though
can be approximated to a LL1 consumption parser using the tokenizer in boost. 

Its bad, boost has a parser called spirit, we should use it, its good. For now this will do
*/
typedef enum{
	LEFTBRACKET,
	TEXT,
	RIGHTBRACKET,
} BasicParse;

typedef enum
{
	STRING, // Its just the string value, whatever that is
	RESOURCE, // Its another resource, load it
	SURFACE, // SDL_Surface
	SOUND, // A SoundSource
	INT,
	DOUBLE,
	INTARR,
}  DataType;

/*
 * utility to help format getting resources from a bundle
 *
 * example:
 * int R = GET_RESOURCE(int, bundle, "colorkey", 0);
 */
#define GET_RESOURCE(CLASS, BUNDLE, STRING, INDEX) ((CLASS*)(BUNDLE)[(STRING)])[(INDEX)]

class ResourceBundle
{

private:
	// readArray template class replaces readIntArray and readFloatArray
	template<class type> static type* readArray(string cstr);
	template<class type> static type** readArrayArray(string cstr);
	ResourceBundle ** ResourceBundle::readResourceArray(string cstr);
	static void registerResource(string, void *);

	static std::map<string,void*> resourceRegister;
	std::map<std::string,void*> data;
	ResourceBundle(char *);
public:
	virtual ~ResourceBundle ();
	void * operator[](const char*);
	void print();
public:	 //The static things
	static int isInit;
	static std::map<std::string,DataType> supportedTypes;
	static void initSupportedTypes();
	static SDL_Surface* loadImage(char *);
	static ResourceBundle* getResource(char*);
};


#endif /* end of include guard: RESOURCEBUNDLE_H_ */
