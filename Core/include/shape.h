/*
 * shape.h
 *
 *  Created on: Feb 12, 2013
 *      Author: Vadim Fedorov
 */

#ifndef SHAPE_H_
#define SHAPE_H_

#include <cmath>
#include "point.h"

struct Shape
{
	// TODO: make it unsigned
	unsigned int size_x, size_y, size_t;

	Shape();
	Shape(unsigned int size_x, unsigned int size_y);
	Shape(unsigned int size_x, unsigned int size_y, unsigned int size_t);
	Shape(const Shape &source);

	bool operator== (const Shape &other) const;
	bool operator!= (const Shape &other) const;

	friend inline bool operator< (const Shape& lhs, const Shape& rhs);
	friend inline bool operator> (const Shape& lhs, const Shape& rhs);
	friend inline bool operator<=(const Shape& lhs, const Shape& rhs);
	friend inline bool operator>=(const Shape& lhs, const Shape& rhs);

	bool is_empty() const;
	bool contains(const Point &p) const;
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