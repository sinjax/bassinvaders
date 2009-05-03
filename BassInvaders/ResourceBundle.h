/*
 *  ResourceBundle.h
 *  BassInvaders
 *
 *  Created by Sina Samangooei on 03/05/2009.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */


#include <map>
#include <yaml>

void operator >> (const YAML::Node& node, Monster& monster);


class ResourceBundle
{
public:
	ResourceBundle();
	virtual ~ResourceBundle ();

	std::map<char*, char*> data;
};
