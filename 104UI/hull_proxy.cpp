/**
 * Copyright (C) 2016, Vadim Fedorov <coderiks@gmail.com>
 *
 * This program is free software: you can use, modify and/or
 * redistribute it under the terms of the simplified BSD
 * License. You should have received a copy of this license along
 * this program. If not, see
 * <http://www.opensource.org/licenses/bsd-license.html>.
 */

/// Created on: Feb 4, 2014

#include "hull_proxy.h"

HullProxy::HullProxy(IHull *proxied_hull)
: _proxied_hull(proxied_hull)
{
	_is_enabled = false;
}


void HullProxy::enable(bool should_enable)
{
	_is_enabled = should_enable;
}


void HullProxy::disable()
{
	_is_enabled = false;
}


SequenceFx<float> HullProxy::request_sequence()
{
	return (_is_enabled) ? _proxied_hull->request_sequence() : SequenceFx<float>();
}


LayerManager* HullProxy::request_layer_manager()
{
	return (_is_enabled) ? _proxied_hull->request_layer_manager() : 0;
}


InteractionManager* HullProxy::request_interaction_manager()
{
	return (_is_enabled) ? _proxied_hull->request_interaction_manager() : 0;
}


Gtk::Box* HullProxy::request_ui_placeholder()
{
	return (_is_enabled) ? _proxied_hull->request_ui_placeholder() : 0;
}


int HullProxy::request_current_time()
{
	return (_is_enabled) ? _proxied_hull->request_current_time() : -1;
}


void HullProxy::request_module(RequestBase<IModule> &request)
{
	// ?TODO: do we really need to check if current proxy is enabled here? Isn't it super safe to request module at any time?
	//if (_is_enabled) {
		_proxied_hull->request_module(request);
	//}
}
