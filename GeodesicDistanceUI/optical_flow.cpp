/*
 * optical_flow.cpp
 *
 *  Created on: Jan 30, 2013
 *      Author: Vadim Fedorov
 */

#include "optical_flow.h"

ColorCode OpticalFlow::s_color_code;
IntensityCode OpticalFlow::s_intensity_code;

OpticalFlow::OpticalFlow()
	:OpticalFlowContainer()
{
	_max_length = -1;
}


OpticalFlow::~OpticalFlow() {}

Glib::RefPtr<Gdk::Pixbuf> OpticalFlow::get_magnitudes_view()
{
	Glib::RefPtr<Gdk::Pixbuf> buffer = get_view(&s_intensity_code);
	return buffer;
}


Glib::RefPtr<Gdk::Pixbuf> OpticalFlow::get_color_code_view()
{
	Glib::RefPtr<Gdk::Pixbuf> buffer = get_view(&s_color_code);
	return buffer;
}

/* private */

Glib::RefPtr<Gdk::Pixbuf> OpticalFlow::get_view(IOptivalFlowCode *optical_flow_code)
{
	Glib::RefPtr<Gdk::Pixbuf> buffer = Gdk::Pixbuf::create(Gdk::COLORSPACE_RGB, false, BITS_PER_CHANNEL, _size_x, _size_y);

	if (!contains_data())
		return buffer;

	int rowstride = buffer->get_rowstride();	// get internal row length
	int number_of_channels = buffer->get_n_channels();
	guint8* data = buffer->get_pixels();

	float max_length = get_max_length();
	if (max_length < 0.001) {
		max_length = 1;		// flow == 0 everywhere
	}

	char *color = new char[3];
	for (int y = 0; y < _size_y; y++) {
		for (int x = 0; x < _size_x; x++) {
			int i = y * _size_x + x;
			optical_flow_code->get_color(_flow_x[i] / max_length, _flow_y[i] / max_length, color);

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

void OpticalFlow::internal_update()
{
	_max_length = -1;
}


int OpticalFlow::get_max_length()
{
	if (_max_length >= 0)
		return _max_length;

	for (int y = 0; y < _size_y; y++) {
		for (int x = 0; x < _size_x; x++) {
			int i = y * _size_x + x;
			float len = sqrt(_flow_x[i]*_flow_x[i] + _flow_y[i]*_flow_y[i]);
			_max_length = std::max(_max_length, len);
		}
	}

	return _max_length;
}


