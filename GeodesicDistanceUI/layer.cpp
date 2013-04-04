/*
 * layer.cpp
 *
 *  Created on: Mar 25, 2013
 *      Author: Vadim Fedorov
 */

#include "layer.h"

Layer::Layer(string id, string display_name)
{
	_id = id;
	_display_name = display_name;
}


string Layer::get_display_name()
{
	return _display_name;
}


string Layer::get_id()
{
	return _id;
}


void Layer::set_drawing_rectangle(int x, int y, int width, int height)
{
	_drawing_x = x;
	_drawing_y = y;
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


void Layer::draw(const Cairo::RefPtr<Cairo::Context>& context)
{

}


