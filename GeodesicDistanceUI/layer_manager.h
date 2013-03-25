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

#include "layer.h"

using namespace std;

class LayerManager
{
public:
	LayerManager();

	string add_layer(Layer* layer);
	Layer* find_layer(string key);
	// TODO: add smth to iterate layers
private:
	map<string, Layer* > _layers_map;
};


#endif /* LAYER_MANAGER_H_ */
