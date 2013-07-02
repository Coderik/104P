/*
 * coarse_to_fine.cpp
 *
 *  Created on: Jan 23, 2013
 *      Author: upf
 */

#include "headers/coarse_to_fine.h"

void CoarseToFine::zoom_size(int nx, int ny, int &nxx, int &nyy, float factor = 0.5)
{
	nxx = (int)((float) nx * factor + 0.5);
	nyy = (int)((float) ny * factor + 0.5);
}


void CoarseToFine::zoom_out(float *in, float *out, int ncol, int nrow, int new_ncol, int new_nrow)
{
	int i, j;
	float ncol1, nrow1, coord_x, coord_y;
	float value, gamma_x, gamma_y;
	float min_x, min_y, max_x, max_y;

	/* sample the image */

	ncol1 = ncol-1;
	nrow1 = nrow-1;

	/* compute gamma factors */

	gamma_x = (float) new_ncol / (float) ncol;
	gamma_y = (float) new_nrow / (float) nrow;

	/* Bounding box */

	min_x = gamma_x / 2.0 - 0.5;
	min_y = gamma_y / 2.0 - 0.5;

	max_x = min_x + (ncol - 1) * gamma_x;
	max_y = min_y + (nrow - 1) * gamma_y;

	/* Set samples in output image. We use here bilinear interpolation. */

	for(i = 0; i < new_nrow; i++) {
		coord_y = (i - min_y) / gamma_y;

		if (coord_y <= 0)
			coord_y = 0;

		if (coord_y >= nrow1)
			coord_y = nrow1;

		for(j = 0; j < new_ncol; j++) {
			coord_x = (j - min_x) / gamma_x;

			if (coord_x <= 0)
				coord_x = 0;

			if (coord_x >= ncol1)
				coord_x = ncol1;

			value = Interpolation::bilinear(in, ncol, nrow, coord_x, coord_y);
			out[i * (int) new_ncol + j] = value;
		}
	}
}


void CoarseToFine::zoom(float *in, float *out, int nx, int ny, float factor = 0.5)
{
	float *tmp;
	float smooth_sigma, *filter;
	int size, nxx, nyy;

	// Get size
	zoom_size(nx, ny, nxx, nyy, factor);

	/* Subsampling filter */
	smooth_sigma = sqrt(1./factor)/sqrt(2.0);
	size = 2 * round(1.5 * smooth_sigma) + 1;

	GaussianKernel kernel(size, 1, 1, smooth_sigma, 0, 0);

	/* Filter image */
	tmp = new float[nx * ny];
	Filtering::separate_convolution(in, tmp, nx, ny, filter, filter, size, size);

	/* Restrict image */
	apply_restriction(tmp, out, nx, ny, nxx, nyy, factor);

	/* Free memory */
	delete [] tmp;
}


void CoarseToFine::apply_restriction(float *in, float *out, int ncol, int nrow, int new_ncol, int new_nrow, float factor = 0.5)
{
	int i, j;
	float coord_x, coord_y;
	float min_x, min_y;
	float value, gamma_x, gamma_y;

	/* compute gamma factors */

	gamma_x = (float) ncol / (float) new_ncol;
	gamma_y = (float) nrow / (float) new_nrow;

	/* min_x and min_y */

	min_x = gamma_x / 2.0 - 0.5;
	min_y = gamma_y / 2.0 - 0.5;

	/* Set samples in output image. We use here bilinear interpolation. */

	for(i = 0; i < new_nrow; i++) {
		coord_y = min_y + i * gamma_y;

		for(j = 0; j < new_ncol; j++) {
			coord_x = min_x + j * gamma_x;

			value = Interpolation::bilinear(in, ncol, nrow, coord_x, coord_y);
			out[i * (int) new_ncol + j] = value;
		}
	}
}



