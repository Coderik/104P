/*
 * layer_manager.h
 *
 *  Created on: Mar 25, 2013
 *      Author: Vadim Fedorov
 */

#ifndef LAYER_MANAGER_H_
#define LAYER_MANAGER_H_

#include <string>
#include <map>
#include <vector>
#include <utility>

#include "layer.h"

using namespace std;

class LayerManager
{
public:
	LayerManager();

	string add_layer(Layer* layer);
	Layer* find_layer(string key);
	vector<Layer* > get_all_layers();
	void set_visibility(bool is_visible);
	void set_current_time(int time);

	template<class T>
	T* find_or_create_layer(string key, string display_name);

	typedef sigc::signal<void> type_signal_layer_changed;
	type_signal_layer_changed signal_layer_changed()
	{
		return _signal_layer_changed;
	}

private:
	typedef pair<Layer*, sigc::connection > type_layer_and_connection;
	map<string, type_layer_and_connection > _layers_map;
	vector<Layer* > _layers_cache;
	bool _is_layers_cache_valid;
	type_signal_layer_changed _signal_layer_changed;
	bool _is_notification_allowed;
	int _current_time;

	void notify_layer_changed();
};

#include "layer_manager.hpp"	// Implementations for template methods

#endif /* LAYER_MANAGER_H_ */
