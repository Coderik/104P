/*
 * point.h
 *
 *  Created on: Feb 11, 2013
 *      Author: Vadim Fedorov
 */

#ifndef POINT_H_
#define POINT_H_

struct Point
{
	int x, y, t;

	Point();
	Point(int x, int y);
	Point(int x, int y, int t);

	Point& operator= (const Point &p);
	Point& operator+= (const Point &p);
	Point& operator-= (const Point &p);
	const Point operator+ (const Point &p) const;
	const Point operator- (const Point &p) const;

	const Point spatial_add (const Point &p) const;
	const Point spatial_subtract (const Point &p) const;
};


#endif /* POINT_H_ */
