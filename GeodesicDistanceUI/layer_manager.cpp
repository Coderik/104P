/*
 * layer_manager.cpp
 *
 *  Created on: Mar 25, 2013
 *      Author: Vadim Fedorov
 */

#include "layer_manager.h"

LayerManager::LayerManager(){
	_layers_map = map<string, Layer* >();
}


string LayerManager::add_layer(Layer* layer)
{
	string key = layer->get_id();
	_layers_map[key] = layer;	// Inserts new or updates existing.
	return key;
}


Layer* LayerManager::find_layer(string key)
{
	map<string, Layer* >::iterator it;
	it = _layers_map.find(key);
	return (it != _layers_map.end()) ? (*it).second : (Layer*)0;
}



