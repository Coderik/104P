/*
 * i_hull.h
 *
 *  Created on: Apr 10, 2013
 *      Author: Vadim Fedorov
 */

#ifndef I_HULL_H_
#define I_HULL_H_

#include <gtkmm/box.h>
#include "sequence.h"
#include "optical_flow_container.h"
#include "layer_manager.h"
#include "interaction_manager.h"
#include "request_module.h"
#include "i_module.h"

class IHull
{
public:
	virtual ~IHull() {}

	virtual Sequence<float>* request_sequence() = 0;
	virtual vector<OpticalFlowContainer*> request_forward_optical_flow() = 0;
	virtual vector<OpticalFlowContainer*> request_backward_optical_flow() = 0;
	virtual bool request_has_optical_flow_data() = 0;
	/* NOTE: NULL pointer is the correct return value,
	 * which means that requested service is currently unavailable. */
	virtual LayerManager* request_layer_manager() = 0;
	virtual InteractionManager* request_interaction_manager() = 0;
	virtual Gtk::Box* request_ui_placeholder() = 0;
	virtual int request_current_time() = 0;
	// TODO: add requests for b.g. worker, ...

	virtual void request_module(RequestBase<IModule> &request) = 0;
};


#endif /* I_HILL_H_ */