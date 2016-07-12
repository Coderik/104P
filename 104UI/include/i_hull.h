/**
 * Copyright (C) 2016, Vadim Fedorov <coderiks@gmail.com>
 *
 * This program is free software: you can use, modify and/or
 * redistribute it under the terms of the simplified BSD
 * License. You should have received a copy of this license along
 * this program. If not, see
 * <http://www.opensource.org/licenses/bsd-license.html>.
 */

/// Created on: Apr 10, 2013

#ifndef I_HULL_H_
#define I_HULL_H_

#include <gtkmm/box.h>
#include "sequence.h"
#include "layer_manager.h"
#include "interaction_manager.h"
#include "request_module.h"
#include "i_module.h"

class IHull
{
public:
	virtual ~IHull() {}

	virtual SequenceFx<float> request_sequence() = 0;
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
