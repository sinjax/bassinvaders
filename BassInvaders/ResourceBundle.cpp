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
map<string,void*> ResourceBundle::resourceRegister;

ResourceBundle* ResourceBundle::getResource(char* file){
	if(ResourceBundle::resourceRegister[file] == 0)
	{
		ResourceBundle ** r = new ResourceBundle*[1];
		r[0] = new ResourceBundle(file);
		ResourceBundle::resourceRegister[file] = r;
	}

	return ((ResourceBundle**)ResourceBundle::resourceRegister[file])[0];
}


void * ResourceBundle::operator[](const char * s)
{
	return this->data[s];
}

ResourceBundle ** ResourceBundle::readResourceArray(string cstr)
{
	tokenizer< escaped_list_separator<char> > tok(cstr);
	vector <char*> holder;
	for(tokenizer<escaped_list_separator<char> >::iterator beg=tok.begin(); beg!=tok.end();++beg)
	{
		string resource = lexical_cast<string>(*beg);
		char * cstr = new char[resource.size()+1];
		strcpy (cstr, resource.c_str());
		holder.push_back(cstr);
	}

	ResourceBundle ** ret = new ResourceBundle*[holder.size()];
	vector<char*>::iterator itVectorData;
	int index = 0;
	for(itVectorData = holder.begin(); itVectorData != holder.end(); itVectorData++)
	{
		ResourceBundle * a = ResourceBundle::getResource(*(itVectorData));
		ret[index++] = a;
	}
	return ret;
}

void ResourceBundle::initSupportedTypes()
{
	ResourceBundle::isInit = 1;
	ResourceBundle::supportedTypes["filename"] = SURFACE;
	ResourceBundle::supportedTypes["music"] = SOUND;

	ResourceBundle::supportedTypes["bodysprite"] = RESOURCE;
	ResourceBundle::supportedTypes["statefiles"] = RESOURCE;

	ResourceBundle::supportedTypes["colorkey"] = INT;
	ResourceBundle::supportedTypes["sheetstartsat"] = INT;
	ResourceBundle::supportedTypes["spritesize"] = INT;
	ResourceBundle::supportedTypes["rect"] = INT;
	ResourceBundle::supportedTypes["health"] = INT;
	ResourceBundle::supportedTypes["attackdamage"] = INT;
	ResourceBundle::supportedTypes["nextstate"] = INT;
	ResourceBundle::supportedTypes["numberofanimationsteps"] = INT;
	ResourceBundle::supportedTypes["ticksperstep"] = INT;
	ResourceBundle::supportedTypes["numberofrects"] = INT;
	ResourceBundle::supportedTypes["state"] = INT;
	ResourceBundle::supportedTypes["numberofstates"] = INT;
}

SDL_Surface * ResourceBundle::loadImage(char * filename)
{
	void * resource = ResourceBundle::resourceRegister[filename];
	if(resource==0)
	{
		cout << "Surface does not exist, loading it now" << endl;
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

	return (SDL_Surface*)resource;

}

ResourceBundle::ResourceBundle(char * infoFile)
{

	if(!ResourceBundle::isInit)
		ResourceBundle::initSupportedTypes();
	cout << "Loading Resource" << infoFile << endl;

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
		char cstr[value.size()+1];
		strcpy (cstr, value.c_str());
		DataType d = ResourceBundle::supportedTypes[key];
		cout << "Loading Resource Variable " << key << ": " << cstr << endl;
		cout << "\tData type " << d << endl;
		void* toAdd = 0;
		switch(d)
		{
			case STRING:
				toAdd = (void*)cstr;
			break;
			case SOUND:
				if(ResourceBundle::resourceRegister[key] == 0)
				{
					ResourceBundle::resourceRegister[key] = this->data[key];
				}

				toAdd = (void*)( ResourceBundle::resourceRegister[key]);
			break;
			case SURFACE:
				toAdd = (void*)(ResourceBundle::loadImage(cstr));
			break;
			case RESOURCE:
					toAdd = (void*)(ResourceBundle::readResourceArray(cstr));
			break;
			case INT:
				toAdd = (void*)this->readArray<int>(value);
				// Read an integer (array or single)
			break;
			case DOUBLE:
				toAdd = (void*)this->readArray<double>(value);
				// Read a double (array or single)
			break;
			default:
				// Unhandled resource datatype, hold the line
				this->data[key] = (void*)cstr;
			break;
		}

		this->data[key] = toAdd;
		sbuffer.clear();
	}
}

void ResourceBundle::print()
{
	ResourceBundle * b = this;
	for(std::map<std::string, void*>::const_iterator it = b->data.begin(); it != b->data.end(); ++it)
	{

		if(b->supportedTypes[it->first] == STRING)
		{
			std::cout << it->first;
			std::cout << " : " << (char*)(it->second) << std::endl;
		}
		if(b->supportedTypes[it->first] == INT)
		{
			std::cout << it->first;
			std::cout << " : " << (int*)(it->second) << std::endl;
		}
		else if(b->supportedTypes[it->first] == RESOURCE)
		{
			std::cout << it->first << ": INNER RESOURCE" << std::endl;
		}
		else{
			std::cout << "OTHER!!" << std::endl;
		}

    }
}

ResourceBundle::~ResourceBundle()
{
}
