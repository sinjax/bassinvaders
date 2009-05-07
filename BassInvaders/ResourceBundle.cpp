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
void * ResourceBundle::operator[](const char * s)
{
	cout << "Accessing: " << s  << " - " << this->data[s] << endl;
	return this->data[s];
}
/*
ResourceBundle ** ResourceBundle::readBundleArray(string cstr)
{
	tokenizer< escaped_list_separator<char> > tok(cstr);
	vector <ResourceBundle *> holder;
	for(tokenizer<escaped_list_separator<char> >::iterator beg=tok.begin(); beg!=tok.end();++beg)
	{
		if(ResourceBundle::resourceRegister[*beg] == 0)
		{
			ResourceBundle::resourceRegister[*beg] = (void*)(new ResourceBundle(*beg))
		} 
		holder.push_back(ResourceBundle::resourceRegister[*beg]);
			
	}
	ResourceBundle ** ret = new ResourceBundle*[holder.size()];
	vector<ResourceBundle*>::iterator itVectorData;
	int index = 0;
	for(itVectorData = holder.begin(); itVectorData != holder.end(); itVectorData++)
	{
		ret[index++] = *(itVectorData);
	}
	return ret;
}
*/
float * ResourceBundle::readFloatArray(string cstr)
{
	tokenizer< escaped_list_separator<char> > tok(cstr);
	vector <float> holder;
	for(tokenizer<escaped_list_separator<char> >::iterator beg=tok.begin(); beg!=tok.end();++beg)
	{
		holder.push_back(lexical_cast<float>(*beg));
	}
	float * ret = new float[holder.size()];
	vector<float>::iterator itVectorData;
	int index = 0;
	for(itVectorData = holder.begin(); itVectorData != holder.end(); itVectorData++)
	{
		float a = *(itVectorData);
		ret[index++] = a;
	}
	return ret;
}

uint32_t * ResourceBundle::readIntArray(string cstr)
{
	tokenizer< escaped_list_separator<char> > tok(cstr);
	vector <uint32_t> holder;
	for(tokenizer<escaped_list_separator<char> >::iterator beg=tok.begin(); beg!=tok.end();++beg)
	{
		holder.push_back(lexical_cast<int>(*beg));
	}
	uint32_t * ret = new uint32_t[holder.size()];
	vector<uint32_t>::iterator itVectorData;
	int index = 0;
	for(itVectorData = holder.begin(); itVectorData != holder.end(); itVectorData++)
	{
		uint32_t a = *(itVectorData);
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
	ResourceBundle::supportedTypes["statefile"] = RESOURCE;
	
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
	ResourceBundle::resourceRegister[infoFile] = this;
	
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
		cout << "Loading Resource Variable " << key << ": " << cstr << endl;
		cout << "\tData type " << d << endl;
		switch(d)
		{
			case STRING:
				cout << "\tA STRING" << endl;
				this->data[key] = (void*)cstr;
			break;
			case SOUND:
				cout << "\tA SOUND" << endl;
				if(ResourceBundle::resourceRegister[key] == 0)
				{
					this->data[key] = (void*)(new SoundSource(cstr));
					ResourceBundle::resourceRegister[key] = this->data[key];
				}
				else
					this->data[key] = ResourceBundle::resourceRegister[key];
			break;
			case SURFACE:
				cout << "\tA SURFACE" << endl;
				this->data[key] = (void*)(ResourceBundle::loadImage(cstr));
			break;
			case RESOURCE:
				cout << "\tA RESOURCE" << endl;
				if(ResourceBundle::resourceRegister[key] == 0)
				{
					cout << "Resource does NOT exist, loading a new one" << endl;
					this->data[key] = (void*)(new ResourceBundle(cstr));
				}
				else
				{
					cout << "Resource does NOT exist, loading a new one" << endl;
					this->data[key] = ResourceBundle::resourceRegister[key];
				}
				
				cout << "Validating the resource: " << key << endl;
				((ResourceBundle*)this->data[key])->print();
			break;
			case INT:
				cout << "\tAN INT" << endl;
				this->data[key] = (void*)this->readIntArray(value);
				// Read an integer (array or single)
			break;
			case DOUBLE:
				cout << "\tA DOUBLE" << endl;
				this->data[key] = (void*)this->readFloatArray(value);
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





