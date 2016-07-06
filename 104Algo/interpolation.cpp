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

#include "interpolation.h"

float Interpolation::bilinear(const ImageFx<float> &input, float x, float y, BoundaryConditionEnum boundary_conditionn)
{
	return bilinear((float*)input.raw(), input.size_x(), input.size_y(), x, y, boundary_conditionn);
}


float Interpolation::bilinear(const float *input, int size_x, int size_y, float x, float y, BoundaryConditionEnum boundary_conditionn)
{
	int id_x = floor(x);
	int id_y = floor(y);

	int ids_x[2], ids_y[2];
	bool out = false;

	// apply the corresponding boundary conditions
	switch(boundary_conditionn) {
		case BoundaryCondition::neumann:
			ids_x[0] = neumann_boundary_condition(id_x, size_x);
			ids_y[0] = neumann_boundary_condition(id_y, size_y);
			ids_x[1] = neumann_boundary_condition(id_x + 1, size_x);
			ids_y[1] = neumann_boundary_condition(id_y + 1, size_y);
			break;

		case BoundaryCondition::periodic:
			ids_x[0] = periodic_boundary_condition(id_x, size_x);
			ids_y[0] = periodic_boundary_condition(id_y, size_y);
			ids_x[1] = periodic_boundary_condition(id_x + 1, size_x);
			ids_y[1] = periodic_boundary_condition(id_y + 1, size_y);
			break;

		case BoundaryCondition::symmetric:
			ids_x[0] = symmetric_boundary_condition(id_x, size_x);
			ids_y[0] = symmetric_boundary_condition(id_y, size_y);
			ids_x[1] = symmetric_boundary_condition(id_x + 1, size_x);
			ids_y[1] = symmetric_boundary_condition(id_y + 1, size_y);
			break;

		case BoundaryCondition::cutting:
			ids_x[0] = cutting_boundary_condition(id_x, size_x, out);
			ids_y[0] = cutting_boundary_condition(id_y, size_y, out);
			ids_x[1] = cutting_boundary_condition(id_x + 1, size_x, out);
			ids_y[1] = cutting_boundary_condition(id_y + 1, size_y, out);
			if (out) {
				return 0.0;
			}
			break;

		default:
			ids_x[0] = neumann_boundary_condition(id_x, size_x);
			ids_y[0] = neumann_boundary_condition(id_y, size_y);
			ids_x[1] = neumann_boundary_condition(id_x + 1, size_x);
			ids_y[1] = neumann_boundary_condition(id_y + 1, size_y);
			break;
	}

	const float p11 = input[ ids_x[0] + size_x * ids_y[0] ];

	float a = x - id_x;
	float b = y - id_y;
	float result = 0.0;

	// interpolate
	if ((!a) || (!b)) {
		if ((!a) && (!b)) {
			result = p11;
		} else {
			if (!a) {
				const float p12 = input[ ids_x[0] + size_x * ids_y[1] ];
				result = (1.0 - b) * p11 + b * p12;
			} else {
				const float p21 = input[ ids_x[1] + size_x * ids_y[0] ];
				result = (1.0 - a) * p11 + a * p21;
			}
		}
	} else {
		float a_1 = 1.0 - a;
		float b_1 = 1.0 - b;
		const float p21 = input[ ids_x[1] + size_x * ids_y[0] ];
		const float p12 = input[ ids_x[0] + size_x * ids_y[1] ];
		const float p22 = input[ ids_x[1] + size_x * ids_y[1] ];
		result = b_1 * (a_1 * p11 + a * p21) + b * (a_1 * p12 + a * p22);
	}

	return result;
}

float Interpolation::bicubic(const ImageFx<float> &input, float x, float y, BoundaryConditionEnum boundary_conditionn)
{
	return bicubic((float*)input.raw(), input.size_x(), input.size_y(), x, y, boundary_conditionn);
}


float Interpolation::bicubic(const float *input, int size_x, int size_y, float x, float y, BoundaryConditionEnum boundary_conditionn)
{
	int id_x = floor(x);
	int id_y = floor(y);

	const int step_x = (x < 0)? -1: 1;
	const int step_y = (y < 0)? -1: 1;

	int ids_x[4], ids_y[4];
	bool out = false;

	// apply the corresponding boundary conditions
	switch(boundary_conditionn) {
		case BoundaryCondition::neumann:
			ids_x[0] = neumann_boundary_condition(id_x - step_x, size_x);
			ids_y[0] = neumann_boundary_condition(id_y - step_y, size_y);
			ids_x[1] = neumann_boundary_condition(id_x, size_x);
			ids_y[1] = neumann_boundary_condition(id_y, size_y);
			ids_x[2] = neumann_boundary_condition(id_x + step_x, size_x);
			ids_y[2] = neumann_boundary_condition(id_y + step_y, size_y);
			ids_x[3] = neumann_boundary_condition(id_x + 2 * step_x, size_x);
			ids_y[3] = neumann_boundary_condition(id_y + 2 * step_y, size_y);
			break;

		case BoundaryCondition::periodic:
			ids_x[0] = periodic_boundary_condition(id_x - step_x, size_x);
			ids_y[0] = periodic_boundary_condition(id_y - step_y, size_y);
			ids_x[1] = periodic_boundary_condition(id_x, size_x);
			ids_y[1] = periodic_boundary_condition(id_y, size_y);
			ids_x[2] = periodic_boundary_condition(id_x + step_x, size_x);
			ids_y[2] = periodic_boundary_condition(id_y + step_y, size_y);
			ids_x[3] = periodic_boundary_condition(id_x + 2 * step_x, size_x);
			ids_y[3] = periodic_boundary_condition(id_y + 2 * step_y, size_y);
			break;

		case BoundaryCondition::symmetric:
			ids_x[0] = symmetric_boundary_condition(id_x - step_x, size_x);
			ids_y[0] = symmetric_boundary_condition(id_y - step_y, size_y);
			ids_x[1] = symmetric_boundary_condition(id_x, size_x);
			ids_y[1] = symmetric_boundary_condition(id_y, size_y);
			ids_x[2] = symmetric_boundary_condition(id_x + step_x, size_x);
			ids_y[2] = symmetric_boundary_condition(id_y + step_y, size_y);
			ids_x[3] = symmetric_boundary_condition(id_x + 2 * step_x, size_x);
			ids_y[3] = symmetric_boundary_condition(id_y + 2 * step_y, size_y);
			break;

		case BoundaryCondition::cutting:
			ids_x[0] = cutting_boundary_condition(id_x - step_x, size_x, out);
			ids_y[0] = cutting_boundary_condition(id_y - step_y, size_y, out);
			ids_x[1] = cutting_boundary_condition(id_x, size_x, out);
			ids_y[1] = cutting_boundary_condition(id_y, size_y, out);
			ids_x[2] = cutting_boundary_condition(id_x + step_x, size_x, out);
			ids_y[2] = cutting_boundary_condition(id_y + step_y, size_y, out);
			ids_x[3] = cutting_boundary_condition(id_x + 2 * step_x, size_x, out);
			ids_y[3] = cutting_boundary_condition(id_y + 2 * step_y, size_y, out);
			if (out) {
				return 0.0;
			}
			break;

		default:
			ids_x[0] = neumann_boundary_condition(id_x - step_x, size_x);
			ids_y[0] = neumann_boundary_condition(id_y - step_y, size_y);
			ids_x[1] = neumann_boundary_condition(id_x, size_x);
			ids_y[1] = neumann_boundary_condition(id_y, size_y);
			ids_x[2] = neumann_boundary_condition(id_x + step_x, size_x);
			ids_y[2] = neumann_boundary_condition(id_y + step_y, size_y);
			ids_x[3] = neumann_boundary_condition(id_x + 2 * step_x, size_x);
			ids_y[3] = neumann_boundary_condition(id_y + 2 * step_y, size_y);
			break;
	}

	// obtain the interpolation points of the image
	const float p11 = input[ ids_x[0] + size_x * ids_y[0] ];
	const float p12 = input[ ids_x[1] + size_x * ids_y[0] ];
	const float p13 = input[ ids_x[2] + size_x * ids_y[0] ];
	const float p14 = input[ ids_x[3] + size_x * ids_y[0] ];

	const float p21 = input[ ids_x[0] + size_x * ids_y[1] ];
	const float p22 = input[ ids_x[1] + size_x * ids_y[1] ];
	const float p23 = input[ ids_x[2] + size_x * ids_y[1] ];
	const float p24 = input[ ids_x[3] + size_x * ids_y[1] ];

	const float p31 = input[ ids_x[0] + size_x * ids_y[2] ];
	const float p32 = input[ ids_x[1] + size_x * ids_y[2] ];
	const float p33 = input[ ids_x[2] + size_x * ids_y[2] ];
	const float p34 = input[ ids_x[3] + size_x * ids_y[2] ];

	const float p41 = input[ ids_x[0] + size_x * ids_y[3] ];
	const float p42 = input[ ids_x[1] + size_x * ids_y[3] ];
	const float p43 = input[ ids_x[2] + size_x * ids_y[3] ];
	const float p44 = input[ ids_x[3] + size_x * ids_y[3] ];

	// create array
	double pol[4][4] = {
		{p11, p21, p31, p41},
		{p12, p22, p32, p42},
		{p13, p23, p33, p43},
		{p14, p24, p34, p44}
	};

	// interpolate
	return bicubic_internal(pol, x - id_x, y - id_y);
}

/* Private */

/**
 * Cubic interpolation in one dimension
 * Parameters:
 * v - interpolation points
 * x - point to be interpolated
 */
double Interpolation::cubic_internal(double v[4], double x)
{
	// NOTE: Search 'Bicubic convolution algorithm' for details.
	return  v[1] + 0.5 * x * (v[2] - v[0] +
		x * (2.0 *  v[0] - 5.0 * v[1] + 4.0 * v[2] - v[3] +
		x * (3.0 * (v[1] - v[2]) + v[3] - v[0])));
}


/**
 * Bicubic interpolation in two dimensions
 * Parameters:
 * p - array containing the interpolation points
 * x - x position to be interpolated
 * y - y position to be interpolated
 */
double Interpolation::bicubic_internal(double p[4][4], double x, double y)
{
	// NOTE: Search 'Bicubic convolution algorithm' for details.
	double v[4];
	v[0] = cubic_internal(p[0], y);
	v[1] = cubic_internal(p[1], y);
	v[2] = cubic_internal(p[2], y);
	v[3] = cubic_internal(p[3], y);
	return cubic_internal(v, x);
}

/**
 * For size = 4 : | 0 0 0 0 | 0 1 2 3 | 3 3 3 3 |
 */
inline int Interpolation::neumann_boundary_condition(int x, int size)
{
	if(x < 0) {
		x = 0;
	} else if (x >= size) {
		x = size - 1;
	}

	return x;
}


/**
 * For size = 4 : | 0 1 2 3 | 0 1 2 3 | 0 1 2 3 |
 */
inline int Interpolation::periodic_boundary_condition(int x, int size)
{
	if(x < 0) {
		x = (size - abs(x) % size) % size;
	} else if(x >= size) {
		x = x % size;
	}

	return x;
}


/**
 * For size = 4 : | 2 3 2 1 | 0 1 2 3 | 2 1 0 1 |
 */
inline int Interpolation::symmetric_boundary_condition(int x, int size)
{
	if(x < 0 || x >= size) {
		const int border = size - 1;
		const int abs_x = abs(x);

		x = ( (abs_x / border) % 2 ) ? border - ( abs_x % border ) : abs_x % border;
	}

	return x;
}

/**
 * For size = 4 : | -4 -3 -2 -1 | 0 1 2 3 | 4 5 6 7 |
 */
inline int Interpolation::cutting_boundary_condition(int x, int size, bool &is_out)
{
	if(x < 0 || x >= size) {
		is_out = true;
	}

	return x;
}


