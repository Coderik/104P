/**
 * Copyright (C) 2016, Vadim Fedorov <coderiks@gmail.com>
 *
 * This program is free software: you can use, modify and/or
 * redistribute it under the terms of the simplified BSD
 * License. You should have received a copy of this license along
 * this program. If not, see
 * <http://www.opensource.org/licenses/bsd-license.html>.
 */

/// Created on: Mar 25, 2013

#ifndef LAYER_MANAGER_H_
#define LAYER_MANAGER_H_

#include <string>
#include <map>
#include <vector>
#include <utility>

#include "layer.h"

class LayerManager
{
public:
	LayerManager();

	std::string add_layer(Layer* layer);
	Layer* find_layer(std::string key);
	std::vector<Layer* > get_all_layers();
	void set_visibility(bool is_visible);
	void set_current_time(int time);

	template<class T>
	T* find_or_create_layer(std::string key, std::string display_name);

	typedef sigc::signal<void> type_signal_layer_changed;
	type_signal_layer_changed signal_layer_changed()
	{
		return _signal_layer_changed;
	}

private:
	typedef std::pair<Layer*, sigc::connection > type_layer_and_connection;
	std::map<std::string, type_layer_and_connection > _layers_map;
	std::vector<Layer* > _layers_cache;
	bool _is_layers_cache_valid;
	type_signal_layer_changed _signal_layer_changed;
	bool _is_notification_allowed;
	int _current_time;

	void notify_layer_changed();
};

#include "../layer_manager.hpp"	// Implementations for template methods

#endif /* LAYER_MANAGER_H_ */
