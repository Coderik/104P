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

#ifndef FIELD_OPERATIONS_H_
#define FIELD_OPERATIONS_H_

#include "filtering.h"


/**
 *
 * Details on how to compute the divergence and the grad(u) can be found in:
 *  [2] A. Chambolle, "An Algorithm for Total Variation Minimization and
 *      Applications", Journal of Mathematical Imaging and Vision, 20: 89-97, 2004
 *
 **/

class FieldOperations
{
public:
	static void divergence(const float *v1, const float *v2, float *div, const int nx, const int ny);
	static void forward_gradient(const float *f, float *fx, float *fy, const int nx, const int ny);
	static void centered_gradient(const float *in,float *dx, float *dy, const int nx, const int ny);
	static void centered_temporal_gradient(const float *prev_frame, const float *next_frame, float *dt, const int length);
	static void centered_convective_derivative(const float *prev_frame, const float *next_frame,
											   const float *bwd_flow, const float *fwd_flow,
											   float *result,
											   const int nx, const int ny);
};

#endif /* FIELD_OPERATIONS_H_ */
