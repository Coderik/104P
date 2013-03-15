/*
 * point.cpp
 *
 *  Created on: Feb 12, 2013
 *      Author: Vadim Fedorov
 */


#include "headers/point.h"


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



