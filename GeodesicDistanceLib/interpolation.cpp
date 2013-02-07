/*
 * interpolation.cpp
 *
 *  Created on: Jan 23, 2013
 *      Author: upf
 */

#include "headers/interpolation.h"

float Interpolation::bilinear(const float *in, int ncol, int nrow, float x, float y)
{
	int l, k, offset;
	float *x0, *x1, *x2, *x3;
	float a, b, b_1, a_1, phi;

	l = floor(x);
	k = floor(y);

	a = x-l;
	b = y-k;

	offset = k*ncol+l;

	a_1 = 1.0 - a;
	b_1 = 1.0 - b;

	x0 = x1 = (float *) in + offset;
	x1++;
	x2 = x3 = x0 + ncol;
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


float Interpolation::bicubic(const float *in, int nx, int ny, float x, float y)
{
	int N, M, ncol, nrow, j, k, offset;
	float *p_float, *p_float1;
	float phi, sh, sv;
	float mem_c[16], *mem_pc[4], **c;
	float mem_u[8], *uh, *uv;
	float mem_conv[4], *tmp_conv;
	float sh2, sh3, sv2, sv3;

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

	uh = mem_u + 1;
	uv = mem_u + 5;

	/* c has range c[-1..2][-1..2] */

	c = mem_pc + 1;

	c[-1] = mem_c + 1;
	c[0]  = mem_c + 5;
	c[1]  = mem_c + 9;
	c[2]  = mem_c + 13;

	/* Memory for tmp_conv[-1..2] */

	tmp_conv = mem_conv + 1;

	/* Initialize variables */

	j = x;
	k = y;

	/* Get nrow and ncol of the image */

	nrow = ny;
	ncol = nx;

	/* Initialize M, N */

	M = nrow - 1;
	N = ncol - 1;

	if ((j <= 0) || (j >= (N - 1)))
	{
		phi = bilinear(in, nx, ny, x, y);
		return(phi);
	}

	if ((k <= 0) || (k >= (M - 1)))
	{
		phi = bilinear(in, nx, ny, x, y);
		return(phi);
	}

	/* Compute distance of (x,y) to (j,k) */

	sh = x - j;
	sv = y - k;

	sv2 = sv * sv;
	sv3 = sv2 * sv;

	sh2 = sh * sh;
	sh3 = sh2 * sh;

	/* Initialze weights associated to the interpolation nodes */

	uh[-1] = (-sh3+2.0*sh2-sh)*0.5;
	uv[-1] = (-sv3+2.0*sv2-sv)*0.5;

	uh[0] = (3.0*sh3-5.0*sh2+2.0)*0.5;
	uv[0] = (3.0*sv3-5.0*sv2+2.0)*0.5;

	uh[1] = (-3.0*sh3+4.0*sh2+sh)*0.5;
	uv[1] = (-3.0*sv3+4.0*sv2+sv)*0.5;

	uh[2] = (sh3 - sh2)*0.5;
	uv[2] = (sv3 - sv2)*0.5;

	/* We are sure that these values cannot fall outside the image */

	offset    = k * ncol + j;
	p_float  = (float *) in + offset;
	p_float1 = p_float + ncol;

	c[0][0] = *p_float;
	c[1][0] = *(p_float+1);
	c[0][1] = *(p_float1);
	c[1][1] = *(p_float1+1);

	/* For the remaining values we have to check if they fall outside the image */

	if(j == 0) {
		c[-1][0] = 3*in[k*ncol] - 3*in[k*ncol+1] + in[k*ncol+2];
		c[-1][1] = 3*in[(k+1)*ncol] - 3*in[(k+1)*ncol+1] + in[(k+1)*ncol+2];
	} else {
		c[-1][0] = in[k*ncol+(j-1)];
		c[-1][1] = in[(k+1)*ncol+(j-1)];
	}

	if (j == (N - 1)) {
		c[2][0] = 3*in[k*ncol+j] - 3*in[k*ncol+(j-1)] + in[k*ncol+(j-2)];
		c[2][1] = 3*in[(k+1)*ncol+j] - 3*in[(k+1)*ncol+(j-1)] + in[(k+1)*ncol+(j-2)];
	} else {
		c[2][0] = in[k*ncol+(j+2)];
		c[2][1] = in[(k+1)*ncol+(j+2)];
	}

	if (k == 0) {
		c[0][-1] = 3*in[j] - 3*in[ncol+j] + in[2*ncol+j];
		c[1][-1] = 3*in[j+1] - 3*in[ncol+j+1] + in[2*ncol+j+1];
	} else {
		c[0][-1] = in[(k-1)*ncol+j];
		c[1][-1] = in[(k-1)*ncol+(j+1)];
	}

	if (k == (M - 1)) {
		c[0][2] = 3*in[k*ncol+j] - 3*in[(k-1)*ncol+j] + in[(k-2)*ncol+j];
		c[1][2] = 3*in[k*ncol+(j+1)] - 3*in[(k-1)*ncol+(j+1)] + in[(k-2)*ncol+(j+1)];
	} else {
		c[0][2] = in[(k+2)*ncol+j];
		c[1][2] = in[(k+2)*ncol+(j+1)];
	}

	if ((j == 0) && (k == 0)) {
		c[-1][-1] = 3*c[0][-1] - 3*c[1][-1] + c[2][-1];
	} else {
		c[-1][-1] = in[(k-1)*ncol+(j-1)];
	}

	if ((j == (N - 1)) && (k == 0)) {
		c[2][-1] = 3 * c[1][-1] - 3 * c[0][-1] + c[-1][-1];
	} else {
		c[2][-1] = in[(k-1)*ncol+(j+2)];
	}

	if ((j == 0) && (k == (M - 1))) {
		c[-1][2] = 3 * c[0][2] - 3 * c[1][2] + c[2][2];
	} else {
		c[-1][2] = in[(k-1)*ncol+(j+2)];
	}

	if ((j == (N - 1)) && (k == (M - 1))) {
		c[2][2] = 3 * c[1][2] - 3 * c[0][2] + c[0][2];
	} else {
		c[2][2] = in[(k+2)*ncol+(j+2)];
	}

	/* Ok, now compute convolution */

	phi = 0;

	for(j = -1; j <= 2; j++) {
		phi = 0;

		for(k = -1; k <= 2; k++)
			phi += c[j][k] * uv[k];

		tmp_conv[j] = phi;
	}

	phi = 0;

	for(j = -1; j <= 2; j++) {
		phi += tmp_conv[j] * uh[j];
	}

	/*
	phi = 0;

	for(j = -1; j <= 2; j++)
	for(k = -1; k <= 2; k++)
	phi += c[j][k] * uh[j] * uv[k];
	*/

	return (phi);
}


