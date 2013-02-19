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

	bool is_empty();
	bool is_inside(const Point &p);
	bool is_abs_inside(const Point &p);

	static Shape empty;
};


#endif /* SHAPE_H_ */
