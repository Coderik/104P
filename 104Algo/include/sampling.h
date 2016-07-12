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

#ifndef SAMPLING_H_
#define SAMPLING_H_

#include <algorithm>
#include <math.h>

#include "image.h"
#include "mask.h"
#include "sequence.h"
#include "mask_sequence.h"
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
	static Image<float> downsample(const ImageFx<float> &in, uint sample_size_x, uint sample_size_y);
	static Image<float> downsample(const ImageFx<float> &in, float factor);
	static Sequence<float> downsample(const SequenceFx<float> &in, float factor);
	static Mask downsample(const MaskFx &in, float factor, float threshold = 0.0);
	static MaskSequence downsample(const MaskSequenceFx &in, float factor, float threshold = 0.0);

	static void upsample(const float* in, float* out, uint size_x, uint size_y, uint sample_size_x, uint sample_size_y);
	static Image<float> upsample(const ImageFx<float> &in, uint sample_size_x, uint sample_size_y);
	static Image<float> upsample(const ImageFx<float> &in, float factor);
	static Sequence<float> upsample(const SequenceFx<float> &in, float factor);

	static int get_sample_size(uint size, float factor);
};


#endif /* SAMPLING_H_ */
