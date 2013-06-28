/*
 * gaussian_kernel.cpp
 *
 *  Created on: Jun 26, 2013
 *      Author: Vadim Fedorov
 */

#include "headers/gaussian_kernel.h"

GaussianKernel::GaussianKernel(unsigned int size_x, unsigned int size_y, unsigned int size_z)
{
	_size_x = size_x;
	_size_y = size_y;
	_size_z = size_z;

	// TODO: force size to be odd values

	_values = (float*)malloc(sizeof(float) * size_x * size_y * size_z);

	build_kernel();
}


GaussianKernel::~GaussianKernel()
{
	delete _values;
}


float GaussianKernel::get(int x, int y, int z)
{

	if ((abs(x) > _size_x / 2) || (abs(y) > _size_y / 2) || (abs(z) > _size_z / 2)) {
		return 0.0;
	}

	return _values[get_index(x + (_size_x / 2), y + (_size_y / 2), z + (_size_z / 2))];
}


float GaussianKernel::get(unsigned int x, unsigned int y, unsigned int z)
{
	if (x >= _size_x || y >= _size_y || z >= _size_z) {
		return 0.0;
	}

	return _values[get_index(x, y, z)];
}

/* Private */

void GaussianKernel::build_kernel()
{
	fill_n(_values, _size_x * _size_y * _size_z, 0.0);
	const float sigma_scale = 6;

	float sigma_x = (float)_size_x / sigma_scale;
	float sigma_y = (float)_size_y / sigma_scale;
	float sigma_z = (float)_size_z / sigma_scale;

	float x_a = - (float)_size_x / 2 - STEP / 2;
	float x_b = (float)1.0 / 2 - STEP / 2;
	float y_a = - (float)_size_y / 2 - STEP / 2;
	float y_b = (float)1.0 / 2 - STEP / 2;
	float z_a = - (float)_size_z / 2 - STEP / 2;
	float z_b = (float)1.0 / 2 - STEP / 2;

	// 'Integrate' first half in 1D case, left-top corner in 2D case, etc. (including the central element)
	if (_size_y == 1 && _size_z == 1) {
		// 1D case
		for (float x = x_a; x < x_b; x+= STEP) {
			int i_x = (x - x_a);

			float val = exp(-0.5 * (pow(x, 2) / pow(sigma_x, 2)));

			_values[get_index(i_x, 0, 0)] += val * STEP;
		}
	} else if (_size_z == 1) {
		// 2D case
		for (float x = x_a; x < x_b; x+= STEP) {
			for (float y = y_a; y < y_b; y+= STEP) {
				int i_x = (x - x_a);
				int i_y = (y - y_a);

				float val = exp(-0.5 * (pow(x, 2) / pow(sigma_x, 2) + pow(y, 2) / pow(sigma_y, 2)));

				_values[get_index(i_x, i_y, 0)] += val * pow(STEP, 2);
			}
		}
	} else {
		// 3D case
		for (float x = x_a; x < x_b; x+= STEP) {
			for (float y = y_a; y < y_b; y+= STEP) {
				for (float z = z_a; z < z_b; z+= STEP) {
					int i_x = (x - x_a);
					int i_y = (y - y_a);
					int i_z = (z - z_a);

					float val = exp(-0.5 * (pow(x, 2) / pow(sigma_x, 2) + pow(y, 2) / pow(sigma_y, 2) +
															pow(z, 2) / pow(sigma_z, 2)));

					_values[get_index(i_x, i_y, i_z)] += val * pow(STEP, 3);
				}
			}
		}
	}

	// Propagate values to the rest parts
	for (unsigned int i = 0; i <= _size_x / 2; i++) {
		for (unsigned int j = 0; j <= _size_y / 2; j++) {
			for (unsigned int k = 0; k <= _size_z / 2; k++) {
				float value = _values[get_index(i, j, k)];
				_values[get_index(_size_x - 1 - i, j, k)] = value;
				if (_size_y > 1) {
					_values[get_index(i, _size_y - 1 - j, k)] = value;
					_values[get_index(_size_x - 1 - i, _size_y - 1 - j, k)] = value;
				}
				if (_size_z > 1) {
					_values[get_index(i, j, _size_z - 1 - k)] = value;
					_values[get_index(_size_x - 1 - i, j, _size_z - 1 - k)] = value;
					_values[get_index(i, _size_y - 1 - j, _size_z - 1 - k)] = value;
					_values[get_index(_size_x - 1 - i, _size_y - 1 - j, _size_z - 1 - k)] = value;
				}
			}
		}
	}

	// Normalize
	float sum = 0.0;
	for (unsigned int i = 0; i < _size_x * _size_y * _size_z; i++) {
		sum += _values[i];
	}
	for (unsigned int i = 0; i < _size_x * _size_y * _size_z; i++) {
		_values[i] /= sum;
	}
}


inline int GaussianKernel::get_index(int x, int y, int z) const
{
	return _size_x * (_size_y * z + y) + x;
}


