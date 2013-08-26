/*
 * patch.cpp
 *
 *  Created on: Aug 17, 2013
 *      Author: Vadim Fedorov
 */

#include "headers/patch.h"

Patch::Patch(Point center)
{
	_center = center;
	_size = Shape::empty;
	_is_shifted = false;
}


Patch::Patch(Point center, Shape size)
{
	_center = center;
	_size = size;
	_is_shifted = false;

	_relative_center.x = _size.size_x / 2;
	_relative_center.y = _size.size_y / 2;
	_relative_center.t = _size.size_t / 2;

	_origin = _center - _relative_center;

	_slice_origins = vector<Point>(size.size_t);
	for (unsigned int i = 0; i < _slice_origins.size(); i++) {
		_slice_origins[i] = Point(_origin.x, _origin.y, _origin.t + i);
	}
}


Point Patch::get_center() const
{
	return _center;
}


Shape Patch::get_size() const
{
	return _size;
}


/**
 * Shifts specified slice from the center.
 */
void Patch::shift_slice(unsigned int slice, int shift_x, int shift_y)
{
	if (_slice_origins.size() > 1 && slice < _slice_origins.size()) {
		_slice_origins[slice].x += shift_x;
		_slice_origins[slice].y += shift_y;

		_is_shifted = true;
	}
}


Point Patch::absolute(Point relative) const
{
	if (_size.is_empty()) {
		return relative;
	} else {
		// ensure 'relative' point lies within the patch
		relative.x = max(0, min((int)_size.size_x - 1, relative.x));
		relative.y = max(0, min((int)_size.size_y - 1, relative.y));
		relative.t = max(0, min((int)_size.size_t - 1, relative.t));

		if (!_is_shifted) {
			return _origin + relative;
		} else {
			return _slice_origins[relative.t].spatial_add(relative);
		}
	}
}


Point Patch::get_slice_center(unsigned int slice) const
{
	if (_size.is_empty()) {
		return _center;
	} else {
		if (slice >= _size.size_t) {
			return Point(-1, -1, -1);
		} else {
			if (!_is_shifted) {
				return _origin + Point(_relative_center.x, _relative_center.y, slice);
			} else {
				return _slice_origins[slice].spatial_add(_relative_center);
			}
		}
	}
}


Point Patch::get_slice_origin(unsigned int slice) const
{
	if (_size.is_empty()) {
		return _center;
	} else {
		if (slice >= _size.size_t) {
			return Point(-1, -1, -1);
		} else {
			if (!_is_shifted) {
				return _origin + Point(0, 0, slice);
			} else {
				return _slice_origins[slice];
			}
		}
	}
}




