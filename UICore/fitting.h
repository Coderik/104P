/*
 * fitting.h
 *
 *  Created on: Apr 11, 2013
 *      Author: Vadim Fedorov
 */

#ifndef FITTING_H_
#define FITTING_H_

#include <string>
#include "rig.h"
#include "layer_manager.h"
#include "interaction_manager.h"

struct Fitting
{
	std::string display_name;
	Rig *rig;
	LayerManager *layer_manager;
	InteractionManager *interaction_manager;


	Fitting() {
		rig = 0;
		layer_manager = 0;
		interaction_manager = 0;
		display_name = std::string();
	}

	Fitting(std::string display_name, Rig *rig) {
		this->rig = rig;
		this->layer_manager = 0;
		interaction_manager = 0;
		this->display_name = display_name;
	}
};


#endif /* FITTING_H_ */
