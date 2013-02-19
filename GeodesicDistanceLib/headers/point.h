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

	Point& operator+= (const Point &p);
	Point& operator-= (const Point &p);
	const Point operator+ (const Point &p) const;
	const Point operator- (const Point &p) const;


};


#endif /* POINT_H_ */
