/*
 * fitting.h
 *
 *  Created on: Apr 11, 2013
 *      Author: Vadim Fedorov
 */

#ifndef FITTING_H_
#define FITTING_H_

#include "i_rig.h";
#include "../layer_manager.h"

struct Fitting
{
	IRig *rig;
	Layer_Manager *layer_manager;

	Fitting() {
		rig = 0;
		layer_manager = 0;
	}
};


#endif /* FITTING_H_ */
