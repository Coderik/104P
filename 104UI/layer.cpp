/**
 * Copyright (C) 2016, Vadim Fedorov <coderiks@gmail.com>
 *
 * This program is free software: you can use, modify and/or
 * redistribute it under the terms of the simplified BSD
 * License. You should have received a copy of this license along
 * this program. If not, see
 * <http://www.opensource.org/licenses/bsd-license.html>.
 */

/// Created on: Mar 25, 2013

#include "layer.h"

Layer::Layer(string id, string display_name)
{
	_id = id;
	_display_name = display_name;
	_is_visible = true;
	_current_time = 0;
}


string Layer::get_display_name()
{
	return _display_name;
}


string Layer::get_id()
{
	return _id;
}


void Layer::set_drawing_size(int width, int height)
{
	_drawing_width = width;
	_drawing_height = height;
}


bool Layer::set_visibitity(bool is_visible)
{
	if (_is_visible == is_visible) {
		return false;
	}

	_is_visible = is_visible;
	_signal_changed.emit();
	return true;
}


void Layer::set_current_time(int time)
{
	_current_time = time;
}


void Layer::draw(const Cairo::RefPtr<Cairo::Context>& context)
{

}

/* Protected */


