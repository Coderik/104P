/*
 * gaussian_weights.cpp
 *
 *  Created on: Nov 22, 2013
 *      Author: Vadim Fedorov
 */

#include "gaussian_weights.h"

namespace GaussianWeights {

inline int get_index(int x, int y, unsigned int size_x);


Sequence<float> calculate_anisotropic_3d(unsigned int size_x, unsigned int size_y, unsigned int size_z,
										 float sigma_x, float sigma_y, float sigma_z)
{
	Sequence<float> weights(size_x, size_y, size_z, 0.0);

	vector<float*> values = vector<float*>(size_z);
	for (unsigned int i = 0; i < size_z; i++) {
		values[i] = weights.frame(i).raw();
	}

	double x_a = - (float)size_x / 2;
	double y_a = - (float)size_y / 2;
	double z_a = - (float)size_z / 2;
	double step = 1.0 / SEGMENTS;
	double half_step = 0.5 / SEGMENTS;

	// 'Integrate' first half in 1D case, left-top corner in 2D case, etc. (including the central element)
	if (size_y == 1 && size_z == 1) {
		// 1D case
		for (unsigned int i = 0; i <= size_x / 2; i++) {
			double value = 0.0;

			for (unsigned int s = 0; s < SEGMENTS; s++) {
				double x = x_a + i + step * s + half_step;
				double partial = exp(-(pow(x, 2) / pow(sigma_x, 2)));
				value += partial;
			}
			values[0][get_index(i, 0, size_x)] = value;
		}
	} else if (size_z == 1) {
		// 2D case
		for (unsigned int i = 0; i <= size_x / 2; i++) {
			for (unsigned int j = 0; j <= size_y / 2; j++) {
				double value = 0.0;
				for (unsigned int s = 0; s < SEGMENTS; s++) {
					double x = x_a + i + step * s + half_step;
					double y = y_a + j + step * s + half_step;
					double partial = exp(-(pow(x, 2) / (2 * pow(sigma_x, 2)) + pow(y, 2) / (2 * pow(sigma_y, 2))));
					value += partial;
				}
				values[0][get_index(i, j, size_x)] = value;
			}
		}
	} else {
		// 3D case
		for (unsigned int i = 0; i <= size_x / 2; i++) {
			for (unsigned int j = 0; j <= size_y / 2; j++) {
				for (unsigned int k = 0; k <= size_z / 2; k++) {
					double value = 0.0;
					for (unsigned int s = 0; s < SEGMENTS; s++) {
						double x = x_a + i + step * s + half_step;
						double y = y_a + j + step * s + half_step;
						double z = z_a + k + step * s + half_step;
						double partial = exp(-(pow(x, 2) / (2 * pow(sigma_x, 2)) + pow(y, 2) / (2 * pow(sigma_y, 2)) + pow(z, 2) / (2 * pow(sigma_z, 2))));
						value += partial;
					}
					values[k][get_index(i, j, size_x)] = value;
				}
			}
		}
	}

	// Propagate values to the rest parts
	for (unsigned int i = 0; i <= size_x / 2; i++) {
		for (unsigned int j = 0; j <= size_y / 2; j++) {
			for (unsigned int k = 0; k <= size_z / 2; k++) {
				float value = values[k][get_index(i, j, size_x)];
				// TODO: update
				values[k][get_index(size_x - 1 - i, j, size_x)] = value;
				if (size_y > 1) {
					values[k][get_index(i, size_y - 1 - j, size_x)] = value;
					values[k][get_index(size_x - 1 - i, size_y - 1 - j, size_x)] = value;
				}
				if (size_z > 1) {
					values[size_z - 1 - k][get_index(i, j, size_x)] = value;
					values[size_z - 1 - k][get_index(size_x - 1 - i, j, size_x)] = value;
					values[size_z - 1 - k][get_index(i, size_y - 1 - j, size_x)] = value;
					values[size_z - 1 - k][get_index(size_x - 1 - i, size_y - 1 - j, size_x)] = value;
				}
			}
		}
	}

	// Normalize
	float sum = 0.0;
	for (unsigned int i = 0; i < size_z; i++) {
		for (unsigned int j = 0; j < size_x * size_y; j++) {
			sum += values[i][j];
		}
	}

	for (unsigned int i = 0; i < size_z; i++) {
		for (unsigned int j = 0; j < size_x * size_y; j++) {
			values[i][j] /= sum;
		}
	}

	return weights;
}


Sequence<float> calculate_isotropic_3d(unsigned int size, float sigma)
{
	return calculate_anisotropic_3d(size, size, size, sigma, sigma, sigma);
}


Image<float> calculate_anisotropic_2d(unsigned int size_x, unsigned int size_y, float sigma_x, float sigma_y)
{
	Image<float> weights(size_x, size_y, 0.0f);
	float *values = weights.raw();

	double x_a = - (float)size_x / 2;
	double y_a = - (float)size_y / 2;
	double step = 1.0 / SEGMENTS;
	double half_step = 0.5 / SEGMENTS;

	// 'Integrate' first half in 1D case, left-top corner in 2D case, etc. (including the central element)
	if (size_y == 1) {
		// 1D case
		for (unsigned int i = 0; i <= size_x / 2; i++) {
			double value = 0.0;

			for (unsigned int s = 0; s < SEGMENTS; s++) {
				double x = x_a + i + step * s + half_step;
				double partial = exp(-(pow(x, 2) / (2 * pow(sigma_x, 2))));
				value += partial;
			}
			values[get_index(i, 0, size_x)] = value;
		}
	} else {
		// 2D case
		for (unsigned int i = 0; i <= size_x / 2; i++) {
			for (unsigned int j = 0; j <= size_y / 2; j++) {
				double value = 0.0;
				for (unsigned int s = 0; s < SEGMENTS; s++) {
					double x = x_a + i + step * s + half_step;
					double y = y_a + j + step * s + half_step;
					double partial = exp(-(pow(x, 2) / (2 * pow(sigma_x, 2)) + pow(y, 2) / (2 * pow(sigma_y, 2))));
					value += partial;
				}
				values[get_index(i, j, size_x)] = value;
			}
		}
	}

	// Propagate values to the rest parts
	for (unsigned int i = 0; i <= size_x / 2; i++) {
		for (unsigned int j = 0; j <= size_y / 2; j++) {
			float value = values[get_index(i, j, size_x)];
			values[get_index(size_x - 1 - i, j, size_x)] = value;
			if (size_y > 1) {
				values[get_index(i, size_y - 1 - j, size_x)] = value;
				values[get_index(size_x - 1 - i, size_y - 1 - j, size_x)] = value;
			}
		}
	}

	// Normalize
	float sum = 0.0;
	for (unsigned int i = 0; i < size_x * size_y; i++) {
		sum += values[i];
	}
	for (unsigned int i = 0; i < size_x * size_y; i++) {
		values[i] /= sum;
	}

	return weights;
}


Image<float> calculate_isotropic_2d(unsigned int size, float sigma)
{
	return calculate_anisotropic_2d(size, size, sigma, sigma);
}


Image<float> calculate_1d(unsigned int size, float sigma)
{
	return calculate_anisotropic_2d(size, 1, sigma, 0);
}


inline int get_index(int x, int y, unsigned int size_x)
{
	return size_x * y + x;
}

}	// namespace GaussianWeights




