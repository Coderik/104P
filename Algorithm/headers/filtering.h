/*
 * filtering.h
 *
 *  Created on: Jul 2, 2013
 *      Author: Vadim Fedorov
 */

#ifndef FILTERING_H_
#define FILTERING_H_

#include <algorithm>
#include <cstring>

#include "image.h"
#include "gaussian_kernel.h"

using namespace std;

class Filtering
{
public:
	static void separate_convolution(const float *in, float *out, int size_x, int size_y, const float *filter_x, const float *filter_y, int filter_x_size, int filter_y_size);
	static void separate_convolution(float *in_out, int size_x, int size_y, const float *filter_x, const float *filter_y, int filter_x_size, int filter_y_size);
	static Image<float>* separate_convolution(const Image<float> &in, const float *filter_x, const float *filter_y, int filter_x_size, int filter_y_size);
	static void separate_convolution(Image<float> &in_out, const float *filter_x, const float *filter_y, int filter_x_size, int filter_y_size);

	static void median(const float *in, float *out, int size_x, int size_y, int window_size);
	static void median(float *in_out, int size_x, int size_y, int window_size);
	static Image<float>* median(const Image<float> &in, int window_size);
	static void median(Image<float> &in_out, int window_size);

	static void gaussian_smoothing(const float *in, float *out, int size_x, int size_y, int kernel_size);
	static void gaussian_smoothing(float *in_out, int size_x, int size_y, int kernel_size);
};



#endif /* FILTERING_H_ */
