/*
 * filtering.cpp
 *
 *  Created on: Jul 2, 2013
 *      Author: Vadim Fedorov
 */

#include "headers/filtering.h"

void Filtering::separate_convolution(const float *in, float *out, int size_x, int size_y, const float *filter_x, const float *filter_y, int filter_x_size, int filter_y_size)
{
	/* initialize temporal buffer */
	float *buffer;
	buffer = new float[size_x * size_y];

	float sum;
	int id;

	/* convolution along x axis */
	int radius = filter_x_size / 2;

	for (int y = 0; y < size_y; y++) {
		for (int x = 0;x < size_x; x++) {
			sum = 0.0;

			for (int i = 0; i < filter_x_size; i++) {
				id = x - radius + i;
				id = max(0, min(size_x - 1, id));

				sum += filter_x[i] * in[y * size_x + id];
			}

			buffer[y * size_x + x] = sum;
		}
	}

	/* convolution along y axis */
	radius = filter_y_size / 2;

	for (int y = 0;y < size_y; y++) {
		for (int x = 0; x < size_x; x++) {
			sum = 0.0;

			for (int i = 0; i < filter_y_size; i++) {
				id = y - radius + i;
				id = max(0, min(size_y - 1, id));

				sum += filter_y[i] * buffer[id * size_x + x];
			}

			out[y * size_x + x] = sum;
		}
	}

	/* free memory */

	delete [] buffer;
}


void Filtering::separate_convolution(float *in_out, int size_x, int size_y, const float *filter_x, const float *filter_y, int filter_x_size, int filter_y_size)
{
	// NOTE: due to the internal buffer it is possible to simply pass 'in_out' as input and output.
	separate_convolution(in_out, in_out, size_x, size_y, filter_x, filter_y, filter_x_size, filter_y_size);
}


Image<float>* Filtering::separate_convolution(const Image<float> &in, const float *filter_x, const float *filter_y, int filter_x_size, int filter_y_size)
{
	int size_x = in.get_size_x();
	int size_y = in.get_size_y();
	Image<float>* output = new Image<float>(size_x, size_y);
	separate_convolution(in.get_raw_data(), output->get_raw_data(), size_x, size_y, filter_x, filter_y, filter_x_size, filter_y_size);

	return output;
}


void Filtering::separate_convolution(Image<float> &in_out, const float *filter_x, const float *filter_y, int filter_x_size, int filter_y_size)
{
	int size_x = in_out.get_size_x();
	int size_y = in_out.get_size_y();
	separate_convolution(in_out.get_raw_data(), size_x, size_y, filter_x, filter_y, filter_x_size, filter_y_size);
}


void Filtering::median(const float *in, float *out, int size_x, int size_y, int window_size)
{
	float window[window_size * window_size];
	int border = window_size / 2;

	for (int x = 0; x < size_y; x++) {
		for (int y = 0;y < size_x; y++) {
			int window_id = 0;

			for(int i = 0; i < window_size; i++) {
				for(int j = 0; j < window_size; j++) {
					int id_x = x - border + j;
					int id_y = y - border + i;

					/* apply symmetric boundary conditions */
					if (id_x < 0)
						id_x = -id_x - 1;

					if (id_x >= size_y)
						id_x = 2 * size_y - id_x - 1;

					if (id_y < 0)
						id_y = -id_y - 1;

					if (id_y >= size_x)
						id_y = 2 * size_x - id_y - 1;

					window[window_id++] = in[id_y * size_y + id_x];
				}
			}

			sort(window, window + window_size * window_size);
			out[y * size_y + x] = window[window_id / 2];
		}
	}
}


void Filtering::median(float *in_out, int size_x, int size_y, int window_size)
{
	float *buffer = new float[size_x * size_y];
	median(in_out, buffer, size_x, size_y, window_size);
	memcpy(in_out, buffer, sizeof(float) * size_x * size_y);
	delete [] buffer;
}


Image<float>* Filtering::median(const Image<float> &in, int window_size)
{
	int size_x = in.get_size_x();
	int size_y = in.get_size_y();
	Image<float>* output = new Image<float>(size_x, size_y);
	median(in.get_raw_data(), output->get_raw_data(), size_x, size_y, window_size);

	return output;
}

void Filtering::median(Image<float> &in_out, int window_size)
{
	int size_x = in_out.get_size_x();
	int size_y = in_out.get_size_y();
	median(in_out.get_raw_data(), size_x, size_y, window_size);
}


void Filtering::gaussian_smoothing(const float *in, float *out, int size_x, int size_y, int kernel_size)
{
	GaussianKernel kernel(kernel_size, 1, 1);
	separate_convolution(in, out, size_x, size_y, kernel.get_raw(), kernel.get_raw(), kernel_size, kernel_size);
}


void Filtering::gaussian_smoothing(float *in_out, int size_x, int size_y, int kernel_size)
{
	/// NOTE: due to the internal buffer in 'separate_convolution' method
	/// it is possible to simply pass 'in_out' as input and output.
	gaussian_smoothing(in_out, in_out, size_x, size_y, kernel_size);
}


