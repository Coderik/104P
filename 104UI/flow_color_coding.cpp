/*
 * flow_color_coding.cpp
 *
 *  Created on: Jan 30, 2013
 *      Author: Vadim Fedorov
 */

#include "flow_color_coding.h"

ColorCode FlowColorCoding::s_color_code;
IntensityCode FlowColorCoding::s_intensity_code;

Glib::RefPtr<Gdk::Pixbuf> FlowColorCoding::get_magnitudes_view(const ImageFx<float> &flow, float max_motion)
{
	Glib::RefPtr<Gdk::Pixbuf> buffer = get_view(flow, &s_intensity_code, max_motion);
	return buffer;
}


Glib::RefPtr<Gdk::Pixbuf> FlowColorCoding::get_color_code_view(const ImageFx<float> &flow, float max_motion)
{
	Glib::RefPtr<Gdk::Pixbuf> buffer = get_view(flow, &s_color_code, max_motion);
	return buffer;
}

/* private */

Glib::RefPtr<Gdk::Pixbuf> FlowColorCoding::get_view(const ImageFx<float> &flow, IOptivalFlowCode *optical_flow_code, float max_motion)
{
	Glib::RefPtr<Gdk::Pixbuf> buffer = Gdk::Pixbuf::create(Gdk::COLORSPACE_RGB, false, BITS_PER_CHANNEL, flow.size_x(), flow.size_y());

	int rowstride = buffer->get_rowstride();	// get internal row length
	int number_of_channels = buffer->get_n_channels();
	guint8* data = buffer->get_pixels();

	float max_length = (max_motion > 0.0f) ? max_motion : get_max_length(flow);
	if (max_length < 0.001) {
		max_length = 1;		// flow == 0 everywhere
	}

	const float *flow_data = flow.raw();
	char *color = new char[3];
	for (uint y = 0; y < flow.size_y(); y++) {
		for (uint x = 0; x < flow.size_x(); x++) {
			int i = y * flow.size_x() + x;
			optical_flow_code->get_color(flow_data[2 * i] / max_length, flow_data[2 * i + 1] / max_length, color);

			for (int c = 0; c < number_of_channels; c++) {
				/// NOTE: 'rowstride' is the length of internal representation of a row
				/// inside the Pixbuf, and it could differ from x_size*number_of_channels.
				int index = y*rowstride + x*number_of_channels + c;
				data[index] = color[c];
			}
		}
	}

	delete[] color;

	return buffer;
}


float FlowColorCoding::get_max_length(const ImageFx<float> &flow)
{
	float max_length = 0.0f;

	const float *flow_data = flow.raw();
	for (uint i = 0; i < flow.size_x() * flow.size_y(); i++) {
		float len = flow_data[2 * i] * flow_data[2 * i] + flow_data[2 * i + 1] * flow_data[2 * i + 1];
		max_length = std::max(max_length, len);
	}

	max_length = std::sqrt(max_length);

	return max_length;
}


