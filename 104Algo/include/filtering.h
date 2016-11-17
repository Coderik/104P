/**
 * Copyright (C) 2016, Vadim Fedorov <coderiks@gmail.com>
 *
 * This program is free software: you can use, modify and/or
 * redistribute it under the terms of the simplified BSD
 * License. You should have received a copy of this license along
 * this program. If not, see
 * <http://www.opensource.org/licenses/bsd-license.html>.
 */

/// Created on: Jul 2, 2013

#ifndef FILTERING_H_
#define FILTERING_H_

#include <algorithm>
#include <cstring>

#include "image.h"
#include "gaussian_weights.h"

// TODO: handle different boundary conditions (symmetric should be default)

class Filtering
{
public:
	static void separate_convolution(const float *in, float *out, int size_x, int size_y, const float *filter_x, const float *filter_y, int filter_x_size, int filter_y_size);
	static void separate_convolution(float *in_out, int size_x, int size_y, const float *filter_x, const float *filter_y, int filter_x_size, int filter_y_size);
	static Image<float> separate_convolution(const ImageFx<float> &in, const float *filter_x, const float *filter_y, int filter_x_size, int filter_y_size);
	static void separate_convolution(Image<float> &in_out, const float *filter_x, const float *filter_y, int filter_x_size, int filter_y_size);

	static void convolution_2d(const float *in, float *out, int size_x, int size_y, const float *filter_2d, int filter_size_x, int filter_size_y);

	static void median(const float *in, float *out, int size_x, int size_y, int window_size);
	static void median(float *in_out, int size_x, int size_y, int window_size);
	static Image<float> median(const ImageFx<float> &in, int window_size);
	static void median(Image<float> &in_out, int window_size);

	static void gaussian_smoothing(const float *in, float *out, int size_x, int size_y, int kernel_size);
	static void gaussian_smoothing(float *in_out, int size_x, int size_y, int kernel_size);
};



#endif /* FILTERING_H_ */
