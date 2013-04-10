/*
 * i_hull.h
 *
 *  Created on: Apr 10, 2013
 *      Author: Vadim Fedorov
 */

#ifndef I_HULL_H_
#define I_HULL_H_

#include "sequence.h"
#include "optical_flow_container.h"

class IHull
{
public:
	virtual ~IHull() {}

	virtual Sequence* request_sequence() = 0;
	virtual vector<OpticalFlowContainer*> request_forward_optical_flow() = 0;
	virtual vector<OpticalFlowContainer*> request_backward_optical_flow() = 0;
	virtual int request_current_time() = 0;
	// TODO: add requests for placeholder, b.g. worker, layer_manager,...
};


#endif /* I_HILL_H_ */
