/**
 * Copyright (C) 2016, Vadim Fedorov <coderiks@gmail.com>
 *
 * This program is free software: you can use, modify and/or
 * redistribute it under the terms of the simplified BSD
 * License. You should have received a copy of this license along
 * this program. If not, see
 * <http://www.opensource.org/licenses/bsd-license.html>.
 */

/// Created on: Apr 30, 2013
/// Contains implementations for template methods

#include "layer_manager.h"

template<class T>
T* LayerManager::find_or_create_layer(std::string key, std::string display_name)
{
	T *layer = dynamic_cast<T* >(this->find_layer(key));
	if (!layer) {
		layer = new T(key, display_name);
		// TODO: handle visibility the Layer_Manager
		layer->set_visibitity(true);
		this->add_layer(layer);
	}

	return layer;
}
