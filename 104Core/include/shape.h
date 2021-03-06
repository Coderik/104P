/**
 * Copyright (C) 2016, Vadim Fedorov <coderiks@gmail.com>
 *
 * This program is free software: you can use, modify and/or
 * redistribute it under the terms of the simplified BSD
 * License. You should have received a copy of this license along
 * this program. If not, see
 * <http://www.opensource.org/licenses/bsd-license.html>.
 */

/// Created on: Feb 12, 2013

#ifndef SHAPE_H_
#define SHAPE_H_

#include <cmath>
#include "point.h"

typedef unsigned int uint;

/**
 * Container for 2d shape (size_x is width, size_y is height) plus duration size_t.
 */
struct Shape
{
	uint size_x, size_y, size_t;

	Shape();
	Shape(uint size_x, uint size_y);
	Shape(uint size_x, uint size_y, uint size_t);
	Shape(const Shape &source);

	bool operator== (const Shape &other) const;
	bool operator!= (const Shape &other) const;

	friend inline bool operator< (const Shape& lhs, const Shape& rhs);
	friend inline bool operator> (const Shape& lhs, const Shape& rhs);
	friend inline bool operator<=(const Shape& lhs, const Shape& rhs);
	friend inline bool operator>=(const Shape& lhs, const Shape& rhs);

	bool is_empty() const;
	bool contains(const Point &p) const;
	bool contains(int x, int y) const;
	bool contains(int x, int y, int t) const;
	bool abs_contains(const Point &p) const;

	static Shape empty;
};

// NOTE: definitions are in header in order to overload two argument versions.
inline bool operator< (const Shape& lhs, const Shape& rhs)
{
	return lhs.size_t < rhs.size_t ||
			(lhs.size_t == rhs.size_t && lhs.size_y < rhs.size_y) ||
			(lhs.size_t == rhs.size_t && lhs.size_y == rhs.size_y && lhs.size_x < rhs.size_x);
}
inline bool operator> (const Shape& lhs, const Shape& rhs) { return operator< (rhs,lhs); }
inline bool operator<= (const Shape& lhs, const Shape& rhs) { return !operator> (lhs,rhs); }
inline bool operator>= (const Shape& lhs, const Shape& rhs) { return !operator< (lhs,rhs); }



#endif /* SHAPE_H_ */
