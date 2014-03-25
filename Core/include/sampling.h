/*
 * sampling.h
 *
 *  Created on: Jan 23, 2013
 *      Author: Vadim Fedorov
 */

#ifndef SAMPLING_H_
#define SAMPLING_H_

#include <algorithm>
#include <math.h>

#include "image.h"
#include "image_mask.h"
#include "sequence.h"
#include "sequence_mask.h"
#include "interpolation.h"
#include "filtering.h"
#include "gaussian_weights.h"

using namespace std;

typedef unsigned int uint;

// TODO: handle different kinds of interpolation (bicubic)
class Sampling
{
public:
	static void downsample(const float* in, float* out, uint size_x, uint size_y, uint sample_size_x, uint sample_size_y);
	static Image<float>* downsample(const Image<float> &in, uint sample_size_x, uint sample_size_y);
	static Image<float>* downsample(const Image<float> &in, float factor);
	static Sequence<float>* downsample(const Sequence<float> &in, float factor);
	static ImageMask* downsample(const ImageMask &in, float factor, float threshold = 0.0);
	static SequenceMask* downsample(const SequenceMask &in, float factor, float threshold = 0.0);

	static void upsample(const float* in, float* out, uint size_x, uint size_y, uint sample_size_x, uint sample_size_y);
	static Image<float>* upsample(const Image<float> &in, uint sample_size_x, uint sample_size_y);
	static Image<float>* upsample(const Image<float> &in, float factor);
	static Sequence<float>* upsample(const Sequence<float> &in, float factor);

	static int get_sample_size(uint size, float factor);
};


#endif /* SAMPLING_H_ */
