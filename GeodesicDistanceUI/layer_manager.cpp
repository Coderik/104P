/*
 * layer_manager.cpp
 *
 *  Created on: Mar 25, 2013
 *      Author: Vadim Fedorov
 */

#include "layer_manager.h"

LayerManager::LayerManager(){
	_layers_map = map<string, Layer* >();
	_is_layers_cache_valid = false;
}


string LayerManager::add_layer(Layer* layer)
{
	string key = layer->get_id();
	_layers_map[key] = layer;	// Inserts new or updates existing.
	_is_layers_cache_valid = false;
	return key;
}


Layer* LayerManager::find_layer(string key)
{
	map<string, Layer* >::iterator it;
	it = _layers_map.find(key);
	return (it != _layers_map.end()) ? (*it).second : (Layer*)0;
}

vector<Layer* > LayerManager::get_all_layers()
{
	if (!_is_layers_cache_valid) {
		_layers_cache = vector<Layer* >(_layers_map.size());
		map<string, Layer* >::iterator it;
		for (it = _layers_map.begin(); it != _layers_map.end(); ++it) {
			_layers_cache.push_back((*it).second);
		}
		_is_layers_cache_valid = true;
	}

	return _layers_cache;
}



