/*
 * distance_transform.h
 *
 * Implementation of "A General Algorithm for Computing
 * Distance Transforms in Linear Time" by Meijster et al.
 *
 *  Created on: Jul 11, 2013
 *      Author: Vadim Fedorov
 */

#ifndef DISTANCE_TRANSFORM_H_
#define DISTANCE_TRANSFORM_H_

#include <vector>

#include "image.h"
#include "image_mask.h"

using namespace std;

namespace Distance {
	enum Enum {
		Euclidean,	// sqrt( (x2 - x1)^2 + (y2 - y1)^2 )
		Manhattan,	// |x2 - x1| + |y2 - y1|
		chessboard	// max( |x2 - x1|, |y2 - y1| )
	};
}
typedef Distance::Enum DistanceEnum;

// TODO: handle bool* as a mask and float* as an output
class DistanceTransform
{
public:

	/**
	 * For each point assigns the distance to the nearest point NOT belonging to the mask
	 * (masked points receive non-zero distances).
	 */
	static Image<float>* calculate(const ImageMask &mask, DistanceEnum distance_type = Distance::Euclidean);

	/**
	 * For each point assigns the distance to the nearest point belonging to the mask
	 * (unmasked points receive non-zero distances).
	 */
	static Image<float>* calculate_inverse(const ImageMask &mask, DistanceEnum distance_type = Distance::Euclidean);

private:
	static Image<float>* calculate_internal(const ImageMask &mask, DistanceEnum distance_type, bool direct);
	static inline float f(int x, int x_i, float g_i, DistanceEnum distance_type);
	static inline int sep(int i, int u, float g_i, float g_u, int inf, DistanceEnum distance_type);
};




#endif /* DISTANCE_TRANSFORM_H_ */
