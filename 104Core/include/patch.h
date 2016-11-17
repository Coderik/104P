/**
 * Copyright (C) 2016, Vadim Fedorov <coderiks@gmail.com>
 *
 * This program is free software: you can use, modify and/or
 * redistribute it under the terms of the simplified BSD
 * License. You should have received a copy of this license along
 * this program. If not, see
 * <http://www.opensource.org/licenses/bsd-license.html>.
 */

/// Created on: Aug 17, 2013

#ifndef PATCH_H_
#define PATCH_H_

#include <vector>
#include "point.h"
#include "shape.h"

class Patch
{
public:
	Patch(Point center);
	Patch(Point center, Shape size);

	Point get_center() const;
	Shape get_size() const;

	void shift_slice(unsigned int slice, int shift_x, int shift_y);

	Point absolute(Point relative) const;
	Point get_slice_center(unsigned int slice) const;
	Point get_slice_origin(unsigned int slice) const;

private:
	std::vector<Point> _slice_origins;
	Point _center, _origin, _relative_center;
	Shape _size;
	bool _is_shifted;
};


#endif /* PATCH_H_ */
