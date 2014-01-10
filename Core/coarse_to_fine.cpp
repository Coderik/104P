/*
 * coarse_to_fine.cpp
 *
 *  Created on: Jan 23, 2013
 *      Author: Vadim Fedorov
 */

#include "coarse_to_fine.h"

void CoarseToFine::downsample(const float* in, float* out, uint size_x, uint size_y, uint sample_size_x, uint sample_size_y)
{
	float factor_x = (float)size_x / sample_size_x;
	float factor_y = (float)size_y / sample_size_y;

	// adjust smoothing filter
	// TODO: handle different factors for x and y dimentions.
	float sigma = sqrt(factor_x / 2);
	int kernel_size = 2 * round(1.5 * sigma) + 1;
	GaussianKernel kernel(kernel_size, 1, 1, sigma, 0, 0);
	const float *filter = kernel.get_raw();

	// smooth image
	float *buffer = new float[size_x * size_y];
	Filtering::separate_convolution(in, buffer, size_x, size_y, filter, filter, kernel_size, kernel_size);

	// subsample
	float min_x = factor_x / 2.0 - 0.5;
	float min_y = factor_y / 2.0 - 0.5;

	for(uint i = 0; i < sample_size_y; i++) {
		float y = min_y + i * factor_y;
		for(uint j = 0; j < sample_size_x; j++) {
			float x = min_x + j * factor_x;

			float value = Interpolation::bilinear(buffer, size_x, size_y, x, y);
			out[i * sample_size_x + j] = value;
		}
	}

	// free memory
	delete [] buffer;
}


Image<float>* CoarseToFine::downsample(const Image<float> &in, uint sample_size_x, uint sample_size_y)
{
	if (sample_size_x >= (uint)in.get_size_x() || sample_size_y >= (uint)in.get_size_y()) {
		return 0;
	}

	Image<float> *out = new Image<float>(sample_size_x, sample_size_y);
	downsample(in.get_raw_data(), out->get_raw_data(), in.get_size_x(), in.get_size_y(), sample_size_x, sample_size_y);
	return out;
}

Image<float>* CoarseToFine::downsample(const Image<float> &in, float factor)
{
	if (factor <= 0 || factor >= 1.0) {
		return 0;
	}

	int sample_size_x = get_sample_size(in.get_size_x(), factor);
	int sample_size_y = get_sample_size(in.get_size_y(), factor);

	Image<float> *out = new Image<float>(sample_size_x, sample_size_y);
	downsample(in.get_raw_data(), out->get_raw_data(), in.get_size_x(), in.get_size_y(), sample_size_x, sample_size_y);
	return out;
}


// TODO: look for other options
ImageMask* CoarseToFine::downsample(const ImageMask &in, float factor, float threshold)
{
	// binary to float
	Image<float> float_mask(in.get_size_x(), in.get_size_y(), 0.0);
	ImageMask::iterator it;
	for (it = in.begin(); it != in.end(); ++it) {
		float_mask.set_value(*it, 1.0);
	}

	Image<float> *sampled_float_mask = CoarseToFine::downsample(float_mask, factor);

	// float to binary
	threshold = fmax(0.0, fmin(1.0, threshold));
	ImageMask *out = new ImageMask(sampled_float_mask->get_size_x(), sampled_float_mask->get_size_y());
	for (int y = 0; y < sampled_float_mask->get_size_y(); y++) {
		for (int x = 0; x < sampled_float_mask->get_size_x(); x++) {
			if (sampled_float_mask->get_value(x, y) > threshold) {
				out->mask(x, y);
			}
		}
	}

	delete sampled_float_mask;

	return out;
}


void CoarseToFine::upsample(const float* in, float* out, uint size_x, uint size_y, uint sample_size_x, uint sample_size_y)
{
	float factor_x = (float) sample_size_x / size_x;
	float factor_y = (float) sample_size_y / size_y;

	// bounding box
	float min_x = factor_x / 2.0 - 0.5;
	float min_y = factor_y / 2.0 - 0.5;

	// set samples in output image
	for(uint i = 0; i < sample_size_y; i++) {
		float y = (i - min_y) / factor_y;
		y = max<float>(0.0, min<float>(size_y - 1, y));

		for(uint j = 0; j < sample_size_x; j++) {
			float x = (j - min_x) / factor_x;
			x = max<float>(0.0, min<float>(size_x - 1, x));

			float value = Interpolation::bilinear(in, size_x, size_y, x, y);
			out[i * sample_size_x + j] = value;
		}
	}
}


Image<float>* CoarseToFine::upsample(const Image<float> &in, uint sample_size_x, uint sample_size_y)
{
	if (sample_size_x <= (uint)in.get_size_x() || sample_size_y <= (uint)in.get_size_y()) {
		return 0;
	}

	Image<float> *out = new Image<float>(sample_size_x, sample_size_y);
	upsample(in.get_raw_data(), out->get_raw_data(), in.get_size_x(), in.get_size_y(), sample_size_x, sample_size_y);
	return out;
}


Image<float>* CoarseToFine::upsample(const Image<float> &in, float factor)
{
	if (factor <= 1.0) {
		return 0;
	}

	int sample_size_x = get_sample_size(in.get_size_x(), factor);
	int sample_size_y = get_sample_size(in.get_size_y(), factor);

	Image<float> *out = new Image<float>(sample_size_x, sample_size_y);
	upsample(in.get_raw_data(), out->get_raw_data(), in.get_size_x(), in.get_size_y(), sample_size_x, sample_size_y);
	return out;
}


int CoarseToFine::get_sample_size(uint size, float factor)
{
	return (int)((float) size * factor + 0.5);
}
