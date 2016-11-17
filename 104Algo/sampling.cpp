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

#include "sampling.h"

void Sampling::downsample(const float* in, float* out, uint size_x, uint size_y, uint sample_size_x, uint sample_size_y)
{
	float factor_x = (float)size_x / sample_size_x;
	float factor_y = (float)size_y / sample_size_y;

	// adjust smoothing filter
	// TODO: handle different factors for x and y dimentions.
	float sigma = sqrt(factor_x / 2);
	int kernel_size = 2 * round(1.5 * sigma) + 1;
	Image<float> kernel = GaussianWeights::calculate_1d(kernel_size, sigma);
	const float *filter = kernel.raw();

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


Image<float> Sampling::downsample(const ImageFx<float> &in, uint sample_size_x, uint sample_size_y)
{
	if (sample_size_x >= (uint)in.size_x() || sample_size_y >= (uint)in.size_y()) {
		return Image<float>();
	}

	Image<float> out(sample_size_x, sample_size_y);
	downsample(in.raw(), out.raw(), in.size_x(), in.size_y(), sample_size_x, sample_size_y);
	return out;
}

Image<float> Sampling::downsample(const ImageFx<float> &in, float factor)
{
	if (factor <= 0 || factor >= 1.0) {
		return Image<float>();
	}

	int sample_size_x = get_sample_size(in.size_x(), factor);
	int sample_size_y = get_sample_size(in.size_y(), factor);

	Image<float> out(sample_size_x, sample_size_y);
	downsample(in.raw(), out.raw(), in.size_x(), in.size_y(), sample_size_x, sample_size_y);
	return out;
}


Sequence<float> Sampling::downsample(const SequenceFx<float> &in, float factor)
{
	if (factor <= 0 || factor >= 1.0) {
		return Image<float>();
	}

	int sample_size_x = get_sample_size(in.size_x(), factor);
	int sample_size_y = get_sample_size(in.size_y(), factor);

	Sequence<float> result(sample_size_x, sample_size_y);
	for (uint t = 0; t < in.size_t(); t++) {
		ImageFx<float> in_frame = in.frame(t);
		Image<float> out_frame(sample_size_x, sample_size_y);
		downsample(in_frame.raw(), out_frame.raw(), in.size_x(), in.size_y(), sample_size_x, sample_size_y);
		result.add(out_frame);
	}
	return result;
}


// TODO: look for other options
Mask Sampling::downsample(const MaskFx &in, float factor, float threshold)
{
	// binary to float
	Image<float> float_mask(in.size_x(), in.size_y(), 0.0f);
	Mask::iterator it;
	for (it = in.begin(); it != in.end(); ++it) {
		float_mask(*it) = 1.0;
	}

	Image<float> sampled_float_mask = Sampling::downsample(float_mask, factor);

	// float to binary
	threshold = fmax(0.0, fmin(1.0, threshold));
	Mask out(sampled_float_mask.size_x(), sampled_float_mask.size_y());
	for (uint y = 0; y < sampled_float_mask.size_y(); y++) {
		for (uint x = 0; x < sampled_float_mask.size_x(); x++) {
			if (sampled_float_mask(x, y) > threshold) {
				out.mask(x, y);
			}
		}
	}

	return out;
}


MaskSequence Sampling::downsample(const MaskSequenceFx &in, float factor, float threshold)
{
	// binary to float
	Sequence<float> float_mask(in.size(), 0.0);
	Mask::iterator it;
	for (uint t = 0; t < in.size_t(); t++) {
		Image<float> frame = float_mask.frame(t);
		for (it = in[t].begin(); it != in[t].end(); ++it) {
			frame(*it) = 1.0;
		}
	}

	Sequence<float> sampled_float_mask = Sampling::downsample(float_mask, factor);

	// float to binary
	threshold = fmax(0.0, fmin(1.0, threshold));
	MaskSequence out(sampled_float_mask.size());
	for (uint t = 0; t < sampled_float_mask.size_t(); t++) {
		Image<float> sampled_float_mask_frame = sampled_float_mask.frame(t);
		Mask out_frame = out.frame(t);
		for (uint y = 0; y < sampled_float_mask.size_y(); y++) {
			for (uint x = 0; x < sampled_float_mask.size_x(); x++) {
				if (sampled_float_mask_frame(x, y) > threshold) {
					out_frame.mask(x, y);
				}
			}
		}
	}

	return out;
}


void Sampling::upsample(const float* in, float* out, uint size_x, uint size_y, uint sample_size_x, uint sample_size_y)
{
	float factor_x = (float) sample_size_x / size_x;
	float factor_y = (float) sample_size_y / size_y;

	// bounding box
	float min_x = factor_x / 2.0 - 0.5;
	float min_y = factor_y / 2.0 - 0.5;

	// set samples in output image
	for(uint i = 0; i < sample_size_y; i++) {
		float y = (i - min_y) / factor_y;
		y = std::max<float>(0.0, std::min<float>(size_y - 1, y));

		for(uint j = 0; j < sample_size_x; j++) {
			float x = (j - min_x) / factor_x;
			x = std::max<float>(0.0, std::min<float>(size_x - 1, x));

			float value = Interpolation::bilinear(in, size_x, size_y, x, y);
			out[i * sample_size_x + j] = value;
		}
	}
}


Image<float> Sampling::upsample(const ImageFx<float> &in, uint sample_size_x, uint sample_size_y)
{
	if (sample_size_x <= in.size_x() || sample_size_y <= in.size_y()) {
		return Image<float>();
	}

	Image<float> out(sample_size_x, sample_size_y);
	upsample(in.raw(), out.raw(), in.size_x(), in.size_y(), sample_size_x, sample_size_y);

	return out;
}


Image<float> Sampling::upsample(const ImageFx<float> &in, float factor)
{
	if (factor <= 1.0) {
		return Image<float>();
	}

	int sample_size_x = get_sample_size(in.size_x(), factor);
	int sample_size_y = get_sample_size(in.size_y(), factor);

	Image<float> out(sample_size_x, sample_size_y);
	upsample(in.raw(), out.raw(), in.size_x(), in.size_y(), sample_size_x, sample_size_y);

	return out;
}


Sequence<float> Sampling::upsample(const SequenceFx<float> &in, float factor)
{
	if (factor <= 1.0) {
		return Image<float>();
	}

	int sample_size_x = get_sample_size(in.size_x(), factor);
	int sample_size_y = get_sample_size(in.size_y(), factor);

	Sequence<float> result(sample_size_x, sample_size_y);
	for (uint t = 0; t < in.size_t(); t++) {
		ImageFx<float> in_frame = in.frame(t);
		Image<float> out_frame(sample_size_x, sample_size_y);
		upsample(in_frame.raw(), out_frame.raw(), in.size_x(), in.size_y(), sample_size_x, sample_size_y);
		result.add(out_frame);
	}

	return result;
}


int Sampling::get_sample_size(uint size, float factor)
{
	return (int)((float) size * factor + 0.5);
}
