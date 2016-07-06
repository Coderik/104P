/**
 * Copyright (C) 2016, Vadim Fedorov <coderiks@gmail.com>
 *
 * This program is free software: you can use, modify and/or
 * redistribute it under the terms of the simplified BSD
 * License. You should have received a copy of this license along
 * this program. If not, see
 * <http://www.opensource.org/licenses/bsd-license.html>.
 */

/// Created on: Apr 30, 2013

#include "rectangular_selection_layer.h"

RectangularSelectionLayer::RectangularSelectionLayer(string id, string display_name)
{
	_id = id;
	_display_name = display_name;
	_width = 0;
	_height = 0;
}


RectangularSelectionLayer::~RectangularSelectionLayer()
{

}


void RectangularSelectionLayer::set_data(int x, int y, int width, int height, int timestamp)
{
	_x = x;
	_y = y;
	_width = width;
	_height = height;
	_timestamp = timestamp;

	_signal_changed.emit();
}


void RectangularSelectionLayer::set_current_time(int time)
{
	_current_time = time;
}


void RectangularSelectionLayer::draw(const Cairo::RefPtr<Cairo::Context>& context)
{
	if (!_is_visible || !_width || !_height) {
		return;
	}

	if (_current_time != _timestamp) {
		return;
	}

	context->save();

	context->set_source_rgba(1.0, 0.0, 0.0, 0.5);
	context->rectangle(_x - 0.5, _y - 0.5, _width, _height);
	context->set_line_width(1.0);
	context->stroke();

	context->restore();
}




