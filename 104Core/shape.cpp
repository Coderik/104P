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

#include "shape.h"

Shape Shape::empty = Shape();

Shape::Shape()
{
	size_x = 0;
	size_y = 0;
	size_t = 0;
}


Shape::Shape(uint size_x, uint size_y)
{
	this->size_x = size_x;
	this->size_y = size_y;
	this->size_t = 1;
}


Shape::Shape(uint size_x, uint size_y, uint size_t)
{
	this->size_x = size_x;
	this->size_y = size_y;
	this->size_t = size_t;
}


Shape::Shape(const Shape &source)
{
	size_x = source.size_x;
	size_y = source.size_y;
	size_t = source.size_t;
}


bool Shape::operator== (const Shape &other) const
{
	return (this->size_x == other.size_x) && (this->size_y == other.size_y) && (this->size_t == other.size_t);
}


bool Shape::operator!= (const Shape &other) const
{
	return !((*this) == other);
}


bool Shape::is_empty() const
{
	return size_x <= 0 || size_y <= 0 || size_t <= 0;
}


/**
 * True if point lies inside the shape (relative to the top left point).
 */
bool Shape::contains(const Point &p) const
{
	return p.x >= 0 && p.y >= 0 && p.t >= 0 && (unsigned)p.x < size_x && (unsigned)p.y < size_y && (unsigned)p.t < size_t;
}


/**
 * True if point lies inside the shape (relative to the top left point).
 */
bool Shape::contains(int x, int y) const
{
	return x >= 0 && y >= 0 && (unsigned)x < size_x && (unsigned)y < size_y;
}


/**
 * True if point lies inside the shape (relative to the top left point).
 */
bool Shape::contains(int x, int y, int t) const
{
	return x >= 0 && y >= 0 && t >= 0 && (unsigned)x < size_x && (unsigned)y < size_y && (unsigned)t < size_t;
}


/**
 * True if point lies inside the shape (relative to the center point).
 */
bool Shape::abs_contains(const Point &p) const
{
	return std::abs((float) p.x) * 2 < size_x && std::abs((float) p.y) * 2 < size_y;
}
