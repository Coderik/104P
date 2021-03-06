/**
 * Copyright (C) 2016, Vadim Fedorov <coderiks@gmail.com>
 *
 * This program is free software: you can use, modify and/or
 * redistribute it under the terms of the simplified BSD
 * License. You should have received a copy of this license along
 * this program. If not, see
 * <http://www.opensource.org/licenses/bsd-license.html>.
 */

/// Created on: Jan 30, 2013

#ifndef FLOW_COLOR_CODING_H_
#define FLOW_COLOR_CODING_H_

#include <glibmm/refptr.h>
#include <gdkmm/pixbuf.h>
#include <math.h>

#include "image.h"
#include "color_code.h"
#include "intensity_code.h"
#include "i_optical_flow_code.h"

class FlowColorCoding
{
public:
	static Glib::RefPtr<Gdk::Pixbuf> get_magnitudes_view(const ImageFx<float> &flow, float max_motion = 0.0f);
	static Glib::RefPtr<Gdk::Pixbuf> get_color_code_view(const ImageFx<float> &flow, float max_motion = 0.0f);

private:
	static float get_max_length(const ImageFx<float> &flow);
	static Glib::RefPtr<Gdk::Pixbuf> get_view(const ImageFx<float> &flow, IOptivalFlowCode *optical_flow_code, float max_motion);

	static ColorCode s_color_code;
	static IntensityCode s_intensity_code;
	static const int BITS_PER_CHANNEL = 8;
};


#endif /* FLOW_COLOR_CODING_H_ */
