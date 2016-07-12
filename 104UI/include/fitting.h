/**
 * Copyright (C) 2016, Vadim Fedorov <coderiks@gmail.com>
 *
 * This program is free software: you can use, modify and/or
 * redistribute it under the terms of the simplified BSD
 * License. You should have received a copy of this license along
 * this program. If not, see
 * <http://www.opensource.org/licenses/bsd-license.html>.
 */

/// Created on: Apr 11, 2013

#ifndef FITTING_H_
#define FITTING_H_

#include <string>
#include "rig.h"
#include "layer_manager.h"
#include "interaction_manager.h"
#include "i_proxy.h"

struct Fitting
{
	std::string display_name;
	Rig *rig;
	IProxy *proxy;
	LayerManager *layer_manager;
	InteractionManager *interaction_manager;


	Fitting() {
		rig = 0;
		proxy = 0;
		layer_manager = 0;
		interaction_manager = 0;
		display_name = std::string();
	}

	Fitting(std::string display_name, Rig *rig) {
		this->rig = rig;
		this->proxy = 0;
		this->layer_manager = 0;
		interaction_manager = 0;
		this->display_name = display_name;
	}
};


#endif /* FITTING_H_ */
