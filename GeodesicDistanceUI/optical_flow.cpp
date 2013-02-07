/*
 * optical_flow.cpp
 *
 *  Created on: Jan 30, 2013
 *      Author: Vadim Fedorov
 */

#include "optical_flow.h"

OpticalFlow::OpticalFlow()
	:OpticalFlowContainer()
{

}


OpticalFlow::~OpticalFlow() {}

Glib::RefPtr<Gdk::Pixbuf> OpticalFlow::get_magnitudes_view()
{
	const int BITS_PER_CHANNEL = 8;

	Glib::RefPtr<Gdk::Pixbuf> buffer = Gdk::Pixbuf::create(Gdk::COLORSPACE_RGB, false, BITS_PER_CHANNEL, _size_x, _size_y);

	if (!contains_data())
		return buffer;

	int rowstride = buffer->get_rowstride();	// get internal row length
	int number_of_channels = buffer->get_n_channels();
	guint8* data = buffer->get_pixels();

	float b = -0.3;	// max value <8
	for (int y = 0; y < _size_y; y++) {
		for (int x = 0; x < _size_x; x++) {
			int i = y * _size_x + x;
			float len = sqrt(_flow_x[i]*_flow_x[i] + _flow_y[i]*_flow_y[i]);

			float value = 255.0 * (1 - exp(b * len));
			if (value > 255)
				value = 255;

			char intensity = (unsigned char)value;
			for (int c = 0; c < number_of_channels; c++) {
				/// NOTE: 'rowstride' is the length of internal representation of a row
				/// inside the Pixbuf, and it could differ from x_size*number_of_channels.
				int index = y*rowstride + x*number_of_channels + c;
				data[index] = intensity;
			}
		}
	}

	return buffer;
}


