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

#ifndef INTERPOLATION_H_
#define INTERPOLATION_H_

#include <math.h>

#include "image.h"

using namespace std;

namespace BoundaryCondition {
	enum Enum {
		neumann,
		periodic,
		symmetric,
		cutting
	};
}
typedef BoundaryCondition::Enum BoundaryConditionEnum;

class Interpolation
{
public:
	static float bilinear(const ImageFx<float> &input, float x, float y,
			BoundaryConditionEnum boundary_condition = BoundaryCondition::neumann);
	static float bilinear(const float *input, int size_x, int size_y, float x, float y,
			BoundaryConditionEnum boundary_condition = BoundaryCondition::neumann);
	static float bicubic(const ImageFx<float> &input, float x, float y,
			BoundaryConditionEnum boundary_condition = BoundaryCondition::neumann);
	static float bicubic(const float *input, int size_x, int size_y, float x, float y,
			BoundaryConditionEnum boundary_condition = BoundaryCondition::neumann);

private:
	static double cubic_internal(double v[4], double x);
	static double bicubic_internal(double p[4][4], double x, double y);
	inline static int neumann_boundary_condition(int x, int size);
	inline static int periodic_boundary_condition(int x, int size);
	inline static int symmetric_boundary_condition(int x, int size);
	inline static int cutting_boundary_condition(int x, int size, bool &is_out);
};


#endif /* INTERPOLATION_H_ */
