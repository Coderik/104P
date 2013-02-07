/*
 * optical_flow.h
 *
 *  Created on: Jan 30, 2013
 *      Author: Vadim Fedorov
 */

#ifndef OPTICAL_FLOW_H_
#define OPTICAL_FLOW_H_

#include <glibmm/refptr.h>
#include <gdkmm/pixbuf.h>
#include <math.h>

#include "optical_flow_container.h"

/**
 * Class extends simple container for optical flow with specific processing methods.
 */
class OpticalFlow : public OpticalFlowContainer
{
public:
	OpticalFlow();
	~OpticalFlow();
	Glib::RefPtr<Gdk::Pixbuf> get_magnitudes_view();

};


#endif /* OPTICAL_FLOW_H_ */
