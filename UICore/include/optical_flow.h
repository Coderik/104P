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
#include "color_code.h"
#include "intensity_code.h"
#include "i_optical_flow_code.h"

/**
 * Class extends simple container for optical flow with specific processing methods.
 */
class OpticalFlow : public OpticalFlowContainer
{
public:
	OpticalFlow();
	virtual ~OpticalFlow();
	Glib::RefPtr<Gdk::Pixbuf> get_magnitudes_view();
	Glib::RefPtr<Gdk::Pixbuf> get_color_code_view();
	virtual void clear();

private:
	virtual void internal_update();
	int get_max_length();
	Glib::RefPtr<Gdk::Pixbuf> get_view(IOptivalFlowCode *optical_flow_code);

	static ColorCode s_color_code;
	static IntensityCode s_intensity_code;
	static const int BITS_PER_CHANNEL = 8;
	float _max_length;
};


#endif /* OPTICAL_FLOW_H_ */
