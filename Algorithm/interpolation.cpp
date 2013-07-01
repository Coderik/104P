/*
 * interpolation.cpp
 *
 *  Created on: Jan 23, 2013
 *      Author: Vadim Fedorov
 */

#include "headers/interpolation.h"

float Interpolation::bilinear(const Image<float> &input, float x, float y)
{
	return bilinear((float*)input.get_raw_data(), input.get_size_x(), input.get_size_y(), x, y);
}


float Interpolation::bilinear(const float *input, int size_x, int size_y, float x, float y)
{
	int l, k, offset;
	float *x0, *x1, *x2, *x3;
	float a, b, b_1, a_1, phi;

	l = floor(x);
	k = floor(y);

	a = x-l;
	b = y-k;

	offset = k*size_x+l;

	a_1 = 1.0 - a;
	b_1 = 1.0 - b;

	x0 = x1 = (float *) input + offset;
	x1++;
	x2 = x3 = x0 + size_x;
	x3++;

	if ((!a) || (!b)) {
		if ((!a) && (!b)) {
			phi = *x0;
		} else {
			if (!a) {
				phi = b_1 * (*x0) + b * (*x2);
			} else {
				phi = a_1 * (*x0) + a * (*x1);
			}
		}
	} else {
		phi = b_1 * (a_1 * (*x0) + a * (*x1)) + b * (a_1 * (*x2) + a * (*x3));
	}

	return(phi);
}

float Interpolation::bicubic(const Image<float> &input, float x, float y, BoundaryConditionEnum boundary_conditionn)
{
	return bicubic((float*)input.get_raw_data(), input.get_size_x(), input.get_size_y(), x, y, boundary_conditionn);
}


float Interpolation::bicubic(const float *input, int size_x, int size_y, float x, float y, BoundaryConditionEnum boundary_conditionn)
{
	// TODO: change variables names
	const int sx = (x < 0)? -1: 1;
	const int sy = (y < 0)? -1: 1;

	int xx, yy, mx, my, dx, dy, ddx, ddy;
	bool out = false;

	//apply the corresponding boundary conditions
	switch(boundary_conditionn) {
		case BoundaryCondition::neumann:
			xx   = neumann_boundary_condition((int) x, size_x);
			yy   = neumann_boundary_condition((int) y, size_y);
			mx  = neumann_boundary_condition((int) x - sx, size_x);
			my  = neumann_boundary_condition((int) y - sx, size_y);
			dx  = neumann_boundary_condition((int) x + sx, size_x);
			dy  = neumann_boundary_condition((int) y + sy, size_y);
			ddx = neumann_boundary_condition((int) x + 2*sx, size_x);
			ddy = neumann_boundary_condition((int) y + 2*sy, size_y);
			break;

		case BoundaryCondition::periodic:
			xx = periodic_boundary_condition((int) x, size_x);
			yy   = periodic_boundary_condition((int) y, size_y);
			mx  = periodic_boundary_condition((int) x - sx, size_x);
			my  = periodic_boundary_condition((int) y - sx, size_y);
			dx  = periodic_boundary_condition((int) x + sx, size_x);
			dy  = periodic_boundary_condition((int) y + sy, size_y);
			ddx = periodic_boundary_condition((int) x + 2*sx, size_x);
			ddy = periodic_boundary_condition((int) y + 2*sy, size_y);
			break;

		case BoundaryCondition::symmetric:
			xx   = symmetric_boundary_condition((int) x, size_x);
			yy   = symmetric_boundary_condition((int) y, size_y);
			mx  = symmetric_boundary_condition((int) x - sx, size_x);
			my  = symmetric_boundary_condition((int) y - sx, size_y);
			dx  = symmetric_boundary_condition((int) x + sx, size_x);
			dy  = symmetric_boundary_condition((int) y + sy, size_y);
			ddx = symmetric_boundary_condition((int) x + 2*sx, size_x);
			ddy = symmetric_boundary_condition((int) y + 2*sy, size_y);
			break;

		case BoundaryCondition::crop:
			xx   = crop_boundary_condition((int) x, size_x, out);
			yy   = crop_boundary_condition((int) y, size_y, out);
			mx  = crop_boundary_condition((int) x - sx, size_x, out);
			my  = crop_boundary_condition((int) y - sx, size_y, out);
			dx  = crop_boundary_condition((int) x + sx, size_x, out);
			dy  = crop_boundary_condition((int) y + sy, size_y, out);
			ddx = crop_boundary_condition((int) x + 2*sx, size_x, out);
			ddy = crop_boundary_condition((int) y + 2*sy, size_y, out);
			if (out) {
				return 0.0;
			}
			break;

		default:
			xx   = neumann_boundary_condition((int) x, size_x);
			yy   = neumann_boundary_condition((int) y, size_y);
			mx  = neumann_boundary_condition((int) x - sx, size_x);
			my  = neumann_boundary_condition((int) y - sx, size_y);
			dx  = neumann_boundary_condition((int) x + sx, size_x);
			dy  = neumann_boundary_condition((int) y + sy, size_y);
			ddx = neumann_boundary_condition((int) x + 2*sx, size_x);
			ddy = neumann_boundary_condition((int) y + 2*sy, size_y);
			break;
	}

	//obtain the interpolation points of the image
	const float p11 = input[mx  + size_x * my];
	const float p12 = input[xx   + size_x * my];
	const float p13 = input[dx  + size_x * my];
	const float p14 = input[ddx + size_x * my];

	const float p21 = input[mx  + size_x * yy];
	const float p22 = input[xx   + size_x * yy];
	const float p23 = input[dx  + size_x * yy];
	const float p24 = input[ddx + size_x * yy];

	const float p31 = input[mx  + size_x * dy];
	const float p32 = input[xx   + size_x * dy];
	const float p33 = input[dx  + size_x * dy];
	const float p34 = input[ddx + size_x * dy];

	const float p41 = input[mx  + size_x * ddy];
	const float p42 = input[xx   + size_x * ddy];
	const float p43 = input[dx  + size_x * ddy];
	const float p44 = input[ddx + size_x * ddy];

	//create array
	double pol[4][4] = {
		{p11, p21, p31, p41},
		{p12, p22, p32, p42},
		{p13, p23, p33, p43},
		{p14, p24, p34, p44}
	};

	//return interpolation
	return bicubic_internal(pol, x-xx, y-yy);







	/*int N, M, ncol, nrow, j, k, offset;
	float *p_float, *p_float1;
	float phi, sh, sv;
	float mem_c[16], *mem_pc[4], **c;
	float mem_u[8], *uh, *uv;
	float mem_conv[4], *tmp_conv;
	float sh2, sh3, sv2, sv3;*/

	/*
	x,y	  : the coordinates of the point to be interpolated
	uh, uv: the horizontal and vertical components of the convolution kernel.
			its values depend on sh, sv respectively.
	mem_u : the space of memory where uv, uh are stored
	sh    : the distance in the horizontal axis between the point to be interpolated
			and the interpolation node.
	sv    : the distance in the vertical axis between the point to be interpolated
			and the interpolation node.
	s     : auxiliar variable which contains the computed distances between the point
			and the interpolation node.
	phi   : the interpolated value of point (x,y).
	c     : the image values of the interpolation nodes.
	mem_c : the memory where the interpolation node values are stored.
	mem_pc: it is interpreted as a vector of pointers to float. This memory is
			used to create the matrix where the interpolation nodes are stored.
	*/

	/* uh, uv has range uv[-1..2] and uh[-1..2] */

	/*uh = mem_u + 1;
	uv = mem_u + 5;*/

	/* c has range c[-1..2][-1..2] */

	/*c = mem_pc + 1;

	c[-1] = mem_c + 1;
	c[0]  = mem_c + 5;
	c[1]  = mem_c + 9;
	c[2]  = mem_c + 13;*/

	/* Memory for tmp_conv[-1..2] */

	/*tmp_conv = mem_conv + 1;*/

	/* Initialize variables */

	/*j = x;
	k = y;*/

	/* Get nrow and ncol of the image */

	/*nrow = size_y;
	ncol = size_x;*/

	/* Initialize M, N */

	/*M = nrow - 1;
	N = ncol - 1;

	if ((j <= 0) || (j >= (N - 1)))
	{
		phi = bilinear(input, size_x, size_y, x, y);
		return(phi);
	}

	if ((k <= 0) || (k >= (M - 1)))
	{
		phi = bilinear(input, size_x, size_y, x, y);
		return(phi);
	}*/

	/* Compute distance of (x,y) to (j,k) */

	/*sh = x - j;
	sv = y - k;

	sv2 = sv * sv;
	sv3 = sv2 * sv;

	sh2 = sh * sh;
	sh3 = sh2 * sh;*/

	/* Initialze weights associated to the interpolation nodes */

	/*uh[-1] = (-sh3+2.0*sh2-sh)*0.5;
	uv[-1] = (-sv3+2.0*sv2-sv)*0.5;

	uh[0] = (3.0*sh3-5.0*sh2+2.0)*0.5;
	uv[0] = (3.0*sv3-5.0*sv2+2.0)*0.5;

	uh[1] = (-3.0*sh3+4.0*sh2+sh)*0.5;
	uv[1] = (-3.0*sv3+4.0*sv2+sv)*0.5;

	uh[2] = (sh3 - sh2)*0.5;
	uv[2] = (sv3 - sv2)*0.5;*/

	/* We are sure that these values cannot fall outside the image */

	/*offset    = k * ncol + j;
	p_float  = (float *) input + offset;
	p_float1 = p_float + ncol;

	c[0][0] = *p_float;
	c[1][0] = *(p_float+1);
	c[0][1] = *(p_float1);
	c[1][1] = *(p_float1+1);*/

	/* For the remaining values we have to check if they fall outside the image */

	/*if(j == 0) {
		c[-1][0] = 3*input[k*ncol] - 3*input[k*ncol+1] + input[k*ncol+2];
		c[-1][1] = 3*input[(k+1)*ncol] - 3*input[(k+1)*ncol+1] + input[(k+1)*ncol+2];
	} else {
		c[-1][0] = input[k*ncol+(j-1)];
		c[-1][1] = input[(k+1)*ncol+(j-1)];
	}

	if (j == (N - 1)) {
		c[2][0] = 3*input[k*ncol+j] - 3*input[k*ncol+(j-1)] + input[k*ncol+(j-2)];
		c[2][1] = 3*input[(k+1)*ncol+j] - 3*input[(k+1)*ncol+(j-1)] + input[(k+1)*ncol+(j-2)];
	} else {
		c[2][0] = input[k*ncol+(j+2)];
		c[2][1] = input[(k+1)*ncol+(j+2)];
	}

	if (k == 0) {
		c[0][-1] = 3*input[j] - 3*input[ncol+j] + input[2*ncol+j];
		c[1][-1] = 3*input[j+1] - 3*input[ncol+j+1] + input[2*ncol+j+1];
	} else {
		c[0][-1] = input[(k-1)*ncol+j];
		c[1][-1] = input[(k-1)*ncol+(j+1)];
	}

	if (k == (M - 1)) {
		c[0][2] = 3*input[k*ncol+j] - 3*input[(k-1)*ncol+j] + input[(k-2)*ncol+j];
		c[1][2] = 3*input[k*ncol+(j+1)] - 3*input[(k-1)*ncol+(j+1)] + input[(k-2)*ncol+(j+1)];
	} else {
		c[0][2] = input[(k+2)*ncol+j];
		c[1][2] = input[(k+2)*ncol+(j+1)];
	}

	if ((j == 0) && (k == 0)) {
		c[-1][-1] = 3*c[0][-1] - 3*c[1][-1] + c[2][-1];
	} else {
		c[-1][-1] = input[(k-1)*ncol+(j-1)];
	}

	if ((j == (N - 1)) && (k == 0)) {
		c[2][-1] = 3 * c[1][-1] - 3 * c[0][-1] + c[-1][-1];
	} else {
		c[2][-1] = input[(k-1)*ncol+(j+2)];
	}

	if ((j == 0) && (k == (M - 1))) {
		c[-1][2] = 3 * c[0][2] - 3 * c[1][2] + c[2][2];
	} else {
		c[-1][2] = input[(k-1)*ncol+(j+2)];
	}

	if ((j == (N - 1)) && (k == (M - 1))) {
		c[2][2] = 3 * c[1][2] - 3 * c[0][2] + c[0][2];
	} else {
		c[2][2] = input[(k+2)*ncol+(j+2)];
	}*/

	/* Ok, now compute convolution */

	/*phi = 0;

	for(j = -1; j <= 2; j++) {
		phi = 0;

		for(k = -1; k <= 2; k++)
			phi += c[j][k] * uv[k];

		tmp_conv[j] = phi;
	}

	phi = 0;

	for(j = -1; j <= 2; j++) {
		phi += tmp_conv[j] * uh[j];
	}*/

	/*
	phi = 0;

	for(j = -1; j <= 2; j++)
	for(k = -1; k <= 2; k++)
	phi += c[j][k] * uh[j] * uv[k];
	*/

	/*return (phi);*/
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
inline int Interpolation::crop_boundary_condition(int x, int size, bool &is_out)
{
	if(x < 0 || x >= size) {
		is_out = true;
	}

	return x;
}


