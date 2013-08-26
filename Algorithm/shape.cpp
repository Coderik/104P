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


Shape::Shape(unsigned int size_x, unsigned int size_y)
{
	this->size_x = size_x;
	this->size_y = size_y;
	size_t = 1;
}


Shape::Shape(unsigned int size_x, unsigned int size_y, unsigned int size_t)
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
 * True if point lies inside the shape (relative to the center point).
 */
bool Shape::abs_contains(const Point &p) const
{
	return std::abs(p.x) * 2 < size_x && std::abs(p.y) * 2 < size_y && std::abs(p.t) * 2 < size_t;
}



