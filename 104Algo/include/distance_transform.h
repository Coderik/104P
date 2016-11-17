/**
 * Copyright (C) 2016, Vadim Fedorov <coderiks@gmail.com>
 *
 * This program is free software: you can use, modify and/or
 * redistribute it under the terms of the simplified BSD
 * License. You should have received a copy of this license along
 * this program. If not, see
 * <http://www.opensource.org/licenses/bsd-license.html>.
 */

/// Created on: Jul 11, 2013
/// Implementation of "A General Algorithm for Computing
/// Distance Transforms in Linear Time" by Meijster et al.

#ifndef DISTANCE_TRANSFORM_H_
#define DISTANCE_TRANSFORM_H_

#include <vector>

#include "image.h"
#include "mask.h"

namespace Distance {
	enum Enum {
		Euclidean,	// sqrt( (x2 - x1)^2 + (y2 - y1)^2 )
		Manhattan,	// |x2 - x1| + |y2 - y1|
		chessboard	// max( |x2 - x1|, |y2 - y1| )
	};
}
typedef Distance::Enum DistanceEnum;

/**
 * Implementation of "A General Algorithm for Computing
 * Distance Transforms in Linear Time" by Meijster et al.
 */
class DistanceTransform
{
public:

	/**
	 * For each point assigns the distance to the nearest point NOT belonging to the mask
	 * (masked points receive non-zero distances).
	 */
	static Image<float> calculate(const MaskFx &mask, DistanceEnum distance_type = Distance::Euclidean);

	/**
	 * For each point assigns the distance to the nearest point belonging to the mask
	 * (unmasked points receive non-zero distances).
	 */
	static Image<float> calculate_inverse(const MaskFx &mask, DistanceEnum distance_type = Distance::Euclidean);

private:
	static Image<float> calculate_internal(const MaskFx &mask, DistanceEnum distance_type, bool direct);
	static inline float f(int x, int x_i, float g_i, DistanceEnum distance_type);
	static inline int sep(int i, int u, float g_i, float g_u, int inf, DistanceEnum distance_type);
};




#endif /* DISTANCE_TRANSFORM_H_ */
