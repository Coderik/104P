/*
 * filtering.cpp
 *
 *  Created on: Jul 2, 2013
 *      Author: Vadim Fedorov
 */

#include "filtering.h"

/**
 * Remember that the filters are ﬂipped in convolution
 */
void Filtering::separate_convolution(const float *in, float *out, int size_x, int size_y, const float *filter_x, const float *filter_y, int filter_x_size, int filter_y_size)
{
	/* initialize temporal buffer */
	float *buffer;
	buffer = new float[size_x * size_y];

	float sum;
	int id;

	/* convolution along x axis */
	int radius = (filter_x_size - 1) / 2;

	for (int y = 0; y < size_y; y++) {
		for (int x = 0;x < size_x; x++) {
			sum = 0.0;

			for (int i = filter_x_size - 1; i >= 0; i--) {
				id = x + radius - i;
				//id = max(0, min(size_x - 1, id));	// neumann boundary conditions

				// symmetric boundary conditions ( | 3 2 1 0 | 0 1 2 3 | 3 2 1 0 | )
				while ((id < 0) || (id >= size_x)) {
					if (id < 0) {
						id = -id - 1;
					}
					if (id >= size_x) {
						id = 2 * size_x - id -1;
					}
				}

				sum += filter_x[i] * in[y * size_x + id];
			}

			buffer[y * size_x + x] = sum;
		}
	}

	/* convolution along y axis */
	radius = (filter_y_size - 1) / 2;

	for (int y = 0;y < size_y; y++) {
		for (int x = 0; x < size_x; x++) {
			sum = 0.0;

			for (int i = filter_y_size - 1; i >= 0; i--) {
				id = y + radius - i;
				//id = max(0, min(size_y - 1, id));	// neumann boundary conditions

				// symmetric boundary conditions ( | 3 2 1 0 | 0 1 2 3 | 3 2 1 0 | )
				while ((id < 0) || (id >= size_y)) {
					if (id < 0) {
						id = -id - 1;
					}
					if (id >= size_y) {
						id = 2 * size_y - id -1;
					}
				}

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


void Filtering::convolution_2d(const float *in, float *out, int size_x, int size_y, const float *filter_2d, int filter_size_x, int filter_size_y)
{
	int filter_center_x = filter_size_x / 2;
	int filter_center_y = filter_size_y / 2;

	for (int y = 0; y < size_y; y++) {
		for (int x = 0; x < size_x; x++) {
			float sum = 0;
			for (int f_y = 0; f_y < filter_size_y; f_y++) {
				for (int f_x = 0; f_x < filter_size_x; f_x++) {
					int f_id = (filter_size_y - 1 - f_y) * filter_size_x + (filter_size_x - 1 - f_x);
					int aux_x = x - filter_center_x + f_x;
					int aux_y = y - filter_center_y + f_y;

					// apply symmetric boundary conditions ( | 3 2 1 0 | 0 1 2 3 | 3 2 1 0 | )
					while ((aux_x < 0) || (aux_x >= size_x)) {
						if (aux_x < 0) {
							aux_x = -aux_x - 1;
						}
						if (aux_x >= size_x) {
							aux_x = 2 * size_x - aux_x -1;
						}
					}
					while ((aux_y < 0) || (aux_y >= size_y)) {
						if (aux_y < 0) {
							aux_y = -aux_y - 1;
						}
						if (aux_y >= size_y) {
							aux_y = 2 * size_y - aux_y -1;
						}
					}

					sum += filter_2d[f_id] * in[aux_y * size_x + aux_x];
				}
			}
			out[y * size_x + x] = sum;
		}
	}
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

					// apply symmetric boundary conditions ( | 3 2 1 0 | 0 1 2 3 | 3 2 1 0 | )
					while ((id_x < 0) || (id_x >= size_x)) {
						if (id_x < 0) {
							id_x = -id_x - 1;
						}
						if (id_x >= size_x) {
							id_x = 2 * size_x - id_x -1;
						}
					}

					while ((id_y < 0) || (id_y >= size_y)) {
						if (id_y < 0) {
							id_y = -id_y - 1;
						}
						if (id_y >= size_y) {
							id_y = 2 * size_y - id_y -1;
						}
					}

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
	float sigma = kernel_size / 6;	// TODO: get rid of magic number
	Image<float> kernel = GaussianWeights::calculate_1d(kernel_size, sigma);
	float* kernel_raw = kernel.get_raw_data();
	separate_convolution(in, out, size_x, size_y, kernel_raw, kernel_raw, kernel_size, kernel_size);
}


void Filtering::gaussian_smoothing(float *in_out, int size_x, int size_y, int kernel_size)
{
	/// NOTE: due to the internal buffer in 'separate_convolution' method
	/// it is possible to simply pass 'in_out' as input and output.
	gaussian_smoothing(in_out, in_out, size_x, size_y, kernel_size);
}


