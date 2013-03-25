/*
 * layer.cpp
 *
 *  Created on: Mar 25, 2013
 *      Author: Vadim Fedorov
 */

#include "layer.h"


Layer::Layer(string id, string display_name)
{
	_id = id;
	_display_name = display_name;
}


string Layer::get_display_name()
{
	return _display_name;
}

string Layer::get_id()
{
	return _id;
}


