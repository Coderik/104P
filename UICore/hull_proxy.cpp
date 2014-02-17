/*
 * hull_proxy.cpp
 *
 *  Created on: Feb 4, 2014
 *      Author: Vadim Fedorov
 */

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


Sequence<float>* HullProxy::request_sequence()
{
	return (_is_enabled) ? _proxied_hull->request_sequence() : 0;
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
	if (_is_enabled) {
		_proxied_hull->request_module(request);
	}
}
