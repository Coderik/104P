/*
 * shape.cpp
 *
 *  Created on: Feb 12, 2013
 *      Author: Vadim Fedorov
 */

#include "headers/shape.h"

Shape Shape::empty = Shape();

Shape::Shape()
{
	size_x = 0;
	size_y = 0;
	size_t = 0;
}


Shape::Shape(int size_x, int size_y)
{
	this->size_x = size_x;
	this->size_y = size_y;
	size_t = 1;
}


Shape::Shape(int size_x, int size_y, int size_t)
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


bool Shape::is_empty() const
{
	return size_x <= 0 || size_y <= 0 || size_t <= 0;
}


/**
 * True if point lies inside the shape (relative to the top left point).
 */
bool Shape::is_inside(const Point &p) const
{
	return p.x >= 0 && p.y >= 0 && p.t >= 0 && p.x < size_x && p.y < size_y && p.t < size_t;
}


/**
 * True if point lies inside the shape (relative to the center point).
 */
bool Shape::is_abs_inside(const Point &p) const
{
	return std::abs(p.x) * 2 < size_x && std::abs(p.y) * 2 < size_y && std::abs(p.t) * 2 < size_t;
}



