/**
 * Copyright (C) 2016, Vadim Fedorov <coderiks@gmail.com>
 *
 * This program is free software: you can use, modify and/or
 * redistribute it under the terms of the simplified BSD
 * License. You should have received a copy of this license along
 * this program. If not, see
 * <http://www.opensource.org/licenses/bsd-license.html>.
 */

/// Created on: Jan 23, 2013

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
