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

#include "layer.h"

using namespace std;

class LayerManager
{
public:
	LayerManager();

	string add_layer(Layer* layer);
	Layer* find_layer(string key);
	vector<Layer* > get_all_layers();
	// get by target
private:
	map<string, Layer* > _layers_map;
	vector<Layer* > _layers_cache;
	bool _is_layers_cache_valid;
};


#endif /* LAYER_MANAGER_H_ */
