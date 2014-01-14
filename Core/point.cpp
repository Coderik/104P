/*
 * point.cpp
 *
 *  Created on: Feb 12, 2013
 *      Author: Vadim Fedorov
 */


#include "point.h"


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
	t = 0;
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



