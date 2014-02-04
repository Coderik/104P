/*
 * hull_proxy.h
 *
 *  Created on: Feb 4, 2014
 *      Author: Vadim Fedorov
 */

#ifndef HULL_PROXY_H_
#define HULL_PROXY_H_

#include "i_proxy.h"
#include "i_hull.h"

class HullProxy : public IProxy, public IHull
{
public:
	HullProxy(IHull *proxied_hull);
	virtual ~HullProxy() {}

	virtual void enable(bool should_enable = true);
	virtual void disable();

	virtual Sequence<float>* request_sequence();
	virtual vector<OpticalFlowContainer*> request_forward_optical_flow();
	virtual vector<OpticalFlowContainer*> request_backward_optical_flow();
	virtual bool request_has_optical_flow_data();

	virtual LayerManager* request_layer_manager();
	virtual InteractionManager* request_interaction_manager();
	virtual Gtk::Box* request_ui_placeholder();
	virtual int request_current_time();

private:
	IHull *_proxied_hull;
	bool _is_enabled;
};





#endif /* HULL_PROXY_H_ */
