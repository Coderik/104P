/*
 * patch_position_layer.cpp
 *
 *  Created on: Apr 3, 2013
 *      Author: Vadim Fedorov
 */

#include "patch_position_layer.h"

Patch_Position_Layer::Patch_Position_Layer(string id, string display_name)
{
	_id = id;
	_display_name = display_name;
	_current_time = 0;
	_patch_size = -1;
}

Patch_Position_Layer::~Patch_Position_Layer()
{
	clear_slice_origins();
}


void Patch_Position_Layer::set_current_time(int time)
{
	_current_time = time;
}


void Patch_Position_Layer::set_patch_size(int patch_size)
{
	_patch_size = patch_size;
}


void Patch_Position_Layer::set_slice_origin(Point slice_origin)
{
	if (_origins.size() <= slice_origin.t) {
		_origins.resize(slice_origin.t + 1, (Point*)0);
	}
	if (_origins[slice_origin.t]) {
		delete _origins[slice_origin.t];
	}
	_origins[slice_origin.t] = new Point(slice_origin);

	_signal_changed.emit();
}


void Patch_Position_Layer::clear_slice_origins()
{
	vector<Point* >::iterator it;
	for(it = _origins.begin(); it != _origins.end(); ++it) {
		if (*it) {
			delete *it;
		}
	}
	_origins.clear();
}


void Patch_Position_Layer::draw(const Cairo::RefPtr<Cairo::Context>& context)
{
	if (!_is_visible || _current_time >= _origins.size() || !_origins[_current_time]) {
		return;
	}

	int x = _drawing_x + _origins[_current_time]->x;
	int y = _drawing_y + _origins[_current_time]->y;

	context->save();

	context->set_source_rgb(1.0, 0.0, 0.0);
	if (_patch_size >= 0) {
		context->move_to(x - 0.5, 				y - 0.5);
		context->line_to(x + _patch_size + 0.5, y - 0.5);
		context->line_to(x + _patch_size + 0.5, y + _patch_size + 0.5);
		context->line_to(x - 0.5, 				y + _patch_size + 0.5);
		context->line_to(x - 0.5, 				y - 0.5);
	} else {
		context->move_to(x + 5, y - 0.5);
		context->line_to(x - 0.5, y - 0.5);
		context->line_to(x - 0.5, y + 5);
	}
	context->set_line_width(1.0);
	context->stroke();

	context->restore();
}




