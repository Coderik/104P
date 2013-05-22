/*
 * layer_manager.hpp - Contains implementations for template methods
 *
 *  Created on: Apr 30, 2013
 *      Author: Vadim Fedorov
 */

#include "layer_manager.h"

template<class T>
T* LayerManager::find_or_create_layer(string key, string display_name)
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
