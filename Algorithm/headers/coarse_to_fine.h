/*
 * coarse_to_fine.h
 *
 *  Created on: Jan 23, 2013
 *      Author: upf
 */

#ifndef COARSE_TO_FINE_H_
#define COARSE_TO_FINE_H_

#include <math.h>

#include "interpolation.h"
#include "filtering.h"
#include "gaussian_kernel.h"

class CoarseToFine
{
public:
	static void zoom_size(int nx, int ny, int &nxx, int &nyy, float factor);
	static void zoom_out(float *in, float *out, int ncol, int nrow, int new_ncol, int new_nrow);
	static void zoom(float *in, float *out, int nx, int ny, float factor);

private:
	// NOTE: original name 'me_image_restriction'
	static void apply_restriction(float *in, float *out, int ncol, int nrow, int new_ncol, int new_nrow, float factor);
};


#endif /* COARSE_TO_FINE_H_ */
