/*
 * coarse_to_fine.h
 *
 *  Created on: Jan 23, 2013
 *      Author: Vadim Fedorov
 */

#ifndef COARSE_TO_FINE_H_
#define COARSE_TO_FINE_H_

#include <algorithm>
#include <math.h>

#include "image.h"
#include "image_mask.h"
#include "interpolation.h"
#include "filtering.h"
#include "gaussian_kernel.h"

using namespace std;

// TODO: [?] rename the class to 'Sampling'
// TODO: handle different kinds of interpolation (bicubic)
class CoarseToFine
{
public:
	static void downsample(const float* in, float* out, uint size_x, uint size_y, uint sample_size_x, uint sample_size_y);
	static Image<float>* downsample(const Image<float> &in, uint sample_size_x, uint sample_size_y);
	static Image<float>* downsample(const Image<float> &in, float factor);
	static ImageMask* downsample(const ImageMask &in, float factor);

	static void upsample(const float* in, float* out, uint size_x, uint size_y, uint sample_size_x, uint sample_size_y);
	static Image<float>* upsample(const Image<float> &in, uint sample_size_x, uint sample_size_y);
	static Image<float>* upsample(const Image<float> &in, float factor);

	static int get_sample_size(uint size, float factor);
};


#endif /* COARSE_TO_FINE_H_ */
