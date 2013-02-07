/*
 * interpolation.h
 *
 *  Created on: Jan 23, 2013
 *      Author: upf
 */

#ifndef INTERPOLATION_H_
#define INTERPOLATION_H_

#include <math.h>

class Interpolation
{
public:
	// NOTE: original name 'me_interpolate_bilinear'
	static float bilinear(const float *in, int ncol, int nrow, float x, float y);
	// NOTE: original name 'me_interpolate_bicubic'
	static float bicubic(const float *in, int nx, int ny, float x, float y);
};


#endif /* INTERPOLATION_H_ */
