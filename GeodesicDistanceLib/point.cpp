/*
 * point.cpp
 *
 *  Created on: Feb 12, 2013
 *      Author: Vadim Fedorov
 */


#include "headers/point.h"

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



