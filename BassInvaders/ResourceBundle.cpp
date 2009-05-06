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
map<string,DataType> ResourceBundle::supportedTypes;
void ResourceBundle::initSupportedTypes()
{
	ResourceBundle::isInit = 1;
	ResourceBundle::supportedTypes["filename"] = SURFACE;
	ResourceBundle::supportedTypes["music"] = SOUND;
	ResourceBundle::supportedTypes["bodysprite"] = RESOURCE;
	ResourceBundle::supportedTypes["colorkey"] = INT;
	ResourceBundle::supportedTypes["sheetstartsat"] = INT;
	ResourceBundle::supportedTypes["spritesize"] = INT;
	ResourceBundle::supportedTypes["rect"] = INT;
	ResourceBundle::supportedTypes["health"] = INT;
	ResourceBundle::supportedTypes["attackdamage"] = INT;
	
	ResourceBundle::supportedTypes["numberofstates"] = NSECTION;
	ResourceBundle::supportedTypes["state"] = SECTION;
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

int * ResourceBundle::readIntArray(string cstr)
{
	tokenizer< escaped_list_separator<char> > tok(cstr);
	vector <int> holder;
	for(tokenizer<escaped_list_separator<char> >::iterator beg=tok.begin(); beg!=tok.end();++beg)
	{
		holder.push_back(lexical_cast<int>(*beg));
	}
	int * ret = new int[holder.size()];
	vector<int>::iterator itVectorData;
	int index = 0;
	for(itVectorData = holder.begin(); itVectorData != holder.end(); itVectorData++)
	{
		int a = *(itVectorData);
		ret[index++] = a;
	}
	return ret;
}

ResourceBundle::ResourceBundle(char * infoFile)
{
	
	if(!ResourceBundle::isInit)
		ResourceBundle::initSupportedTypes();
	
	ifstream inFile;
	inFile.open(infoFile);
	if(!inFile)
	{
		printf("Couldn't open file\n");
		return;
	}
	
	string sbuffer;
	while(getline(inFile,sbuffer))
	{
		string::size_type loc = sbuffer.find( ":", 0 );
		string key = sbuffer.substr(0, loc);
		string value = sbuffer.substr(loc+1,sbuffer.find( "\n", 0 ));
		
		string::size_type startpos = value.find_first_not_of(" "); // Find the first character position after excluding leading blank spaces  
		string::size_type endpos = value.find_last_not_of(" ");
		value = value.substr( startpos, endpos-startpos+1 ); 
		char*cstr = new char [value.size()+1];
		strcpy (cstr, value.c_str());
		DataType d = ResourceBundle::supportedTypes[key];
		
		switch(d)
		{
			case STRING:
				this->data[key] = (void*)cstr;
			break;
			case SOUND:
				this->data[key] = (void*)(new SoundSource(cstr));
			break;
			case SURFACE:
				this->data[key] = (void*)(ResourceBundle::loadImage(cstr));
			break;
			case RESOURCE:
				this->data[key] = (void*)(new ResourceBundle(cstr));
			break;
			case INT:
				this->data[key] = (void*)this->readIntArray(value);
				// Read an integer (array or single)
			break;
			case DOUBLE:
				// Read a double (array or single)
			break;
			default:
				// Unhandled resource datatype, hold the line
				this->data[key] = (void*)cstr;
			break;				
		}
		sbuffer.clear();
	}
}
ResourceBundle::~ResourceBundle()
{
}





