/*
 * warping.h
 *
 *  Created on: Jan 23, 2013
 *      Author: upf
 */

#ifndef WARPING_H_
#define WARPING_H_

#include "interpolation.h"

class Warping
{
public:
	static void warp(const float *input, const float *u, const float *v,
					 float *output, const int nx, const int ny);
};


#endif /* WARPING_H_ */
