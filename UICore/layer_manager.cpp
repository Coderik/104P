/*
 * layer_manager.cpp
 *
 *  Created on: Mar 25, 2013
 *      Author: Vadim Fedorov
 */

#include "layer_manager.h"

LayerManager::LayerManager(){
	_layers_map = map<string, type_layer_and_connection >();
	_is_layers_cache_valid = false;
	_is_notification_allowed = true;
}


string LayerManager::add_layer(Layer* layer)
{
	string key = layer->get_id();

	// Disconnect overridden item from 'notify_layer_changed'.
	map<string, type_layer_and_connection >::iterator layer_it = _layers_map.find(key);
	if (layer_it != _layers_map.end()) {
		layer_it->second.second.disconnect();
	}

	// Connect new item to 'notify_layer_changed' and store.
	sigc::connection connection =
			layer->signal_changed().connect( sigc::mem_fun(*this, &LayerManager::notify_layer_changed) );
	_layers_map[key] = std::make_pair(layer, connection);		// Note: [] operator inserts new or updates existing.
	_is_layers_cache_valid = false;

	return key;
}


Layer* LayerManager::find_layer(string key)
{
	map<string, type_layer_and_connection >::iterator it;
	it = _layers_map.find(key);
	return (it != _layers_map.end()) ? it->second.first : (Layer*)0;
}

vector<Layer* > LayerManager::get_all_layers()
{
	if (!_is_layers_cache_valid) {
		_layers_cache = vector<Layer* >();
		_layers_cache.reserve(_layers_map.size());
		map<string, type_layer_and_connection >::iterator it;
		for (it = _layers_map.begin(); it != _layers_map.end(); ++it) {
			Layer *layer = it->second.first;
			_layers_cache.push_back(layer);
		}
		_is_layers_cache_valid = true;
	}

	return _layers_cache;
}


/**
 * Sets visibility property for all layers.
 * If visibility of any layer is actually affected, emits signal_layer_changed.
 */
void LayerManager::set_visibility(bool is_visible)
{
	bool has_effect = false;

	_is_notification_allowed = false;
	map<string, type_layer_and_connection >::iterator it;
	for (it = _layers_map.begin(); it != _layers_map.end(); ++it) {
		has_effect |= it->second.first->set_visibitity(is_visible);
	}
	_is_notification_allowed = true;

	if (has_effect) {
		notify_layer_changed();
	}
}


/* private */
void LayerManager::notify_layer_changed()
{
	if (_is_notification_allowed) {
		_signal_layer_changed.emit();
	}
}



