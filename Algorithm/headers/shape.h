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
	int size_x, size_y, size_t;

	Shape();
	Shape(int size_x, int size_y);
	Shape(int size_x, int size_y, int size_t);
	Shape(const Shape &source);

	bool operator== (const Shape &other) const;
	bool operator!= (const Shape &other) const;

	bool is_empty() const;
	bool contains(const Point &p) const;
	bool abs_contains(const Point &p) const;

	static Shape empty;
};


#endif /* SHAPE_H_ */
