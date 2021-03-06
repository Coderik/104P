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

#include "point.h"

Point Point::empty = Point(-1, -1);


Point::Point()
{
	x = 0;
	y = 0;
	t = 0;
}


Point::Point(int x, int y)
{
	this->x = x;
	this->y = y;
	this->t = 0;
}


Point::Point(int x, int y, int t)
{
	this->x = x;
	this->y = y;
	this->t = t;
}


bool Point::operator== (const Point &p) const
{
	return (this->x == p.x) && (this->y == p.y) && (this->t == p.t);
}


bool Point::operator!= (const Point &p) const
{
	return !((*this) == p);
}


Point& Point::operator= (const Point &p)
{
	if (this != &p) {
		this->x = p.x;
		this->y = p.y;
		this->t = p.t;
	}

	return *this;
}


Point& Point::operator+= (const Point &p)
{
	x += p.x;
	y += p.y;
	t += p.t;
	return *this;
}


Point& Point::operator-= (const Point &p)
{
	x -= p.x;
	y -= p.y;
	t -= p.t;
	return *this;
}

const Point Point::operator+ (const Point &p) const
{
	Point result = *this;
	result += p;
	return result;
}


const Point Point::operator- (const Point &p) const
{
	Point result = *this;
	result -= p;
	return result;
}


/**
 * Sums only spatial (x and y) components and keeps temporal (t) component
 */
const Point Point::spatial_add (const Point &p) const
{
	Point result = *this;
	result.x += p.x;
	result.y += p.y;
	return result;
}


/**
 * Subtracts only spatial (x and y) components and keeps temporal (t) component
 */
const Point Point::spatial_subtract (const Point &p) const
{
	Point result = *this;
	result.x -= p.x;
	result.y -= p.y;
	return result;
}
