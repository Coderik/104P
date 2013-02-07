/*
 * util.cpp
 *
 *  Created on: Jan 25, 2013
 *      Author: upf
 */

#include "headers/util.h"

float *me_sgauss(float std, int n)
{
	int i, shift;
	float sum,v;
	float *out;

	v = (0.5*(float)(n-1))/(float)(std);
	v = 0.5*v*v/log(10.);

	out = new float[n];
	if (!out) {
		std::cerr << "Not enough memory for out in sgauss." << std::endl;
		exit(1);
	}

	shift = - 0.5*(float)(n-1);

	if (n==1) {
		out[0]=1.0;
	} else {
		/* store Gaussian signal */
		for (i=(n+1)/2;i--;) {
			v = ((float)i+(float)shift)/(float)std;
			out[i] = out[n-1-i] = exp(-0.5*v*v);
		}
		/* normalize to get unit mass */
		for (sum=0.0,i=n;i--;) {
			sum += (float)out[i];
		}
		for (i=n;i--;) {
			out[i] /= sum;
		}
	}

	return out;
}


void me_sepconvol(float *in, float *out, int nx, int ny, float *filter_x, float *filter_y, int size_x, int size_y)
{
	float *tmp;
	int nx1, ny1, x, y, org, i, s;

	float sum;

	nx1 = nx - 1;
	ny1 = ny - 1;

	/* Initialize temporal image */

	tmp = new float[nx*ny];

	/* convolution along x axis */

	org = (size_x - 1) >> 1;

	for (y = 0; y <= ny1; y++) {
		for (x = 0;x <= nx1; x++) {
			sum = 0.0;

			for (i=size_x;i--;) {
				s = x-(i-org);

				while ((s < 0) || (s > nx1)) {
					if (s < 0) {
						s = 0-s-1;
					}

					if (s > nx1) {
						s = nx1-(s-nx1)+1;
					}
				}

				sum += filter_x[i] * in[y*nx+s];
			}

			tmp[y*nx+x] = sum;
		}
	}

	/* convolution along y axis */

	org = (size_y - 1) >> 1;

	for (y = 0;y <= ny1; y++) {
		for (x = 0; x <= nx1; x++) {
			sum = 0.0;

			for (i=size_y;i--;) {
				s = y-(i-org);

				while ((s < 0) || (s > ny1)) {
					if (s < 0) {
						s = 0-s-1;
					}

					if (s > ny1) {
						s = ny1-(s-ny1)+1;
					}
				}

				sum += filter_y[i] * tmp[s*nx+x];
			}

			out[y*nx+x] = sum;
		}
	}

	/* Free memory */

	delete [] tmp;
}


void me_prefilter(float *I0, float *I1, int nx, int ny)
{
   int size;
   float *filter;

   size = 3;
   filter = me_sgauss(1.0, size);

   me_sepconvol(I0, I0, nx, ny, filter, filter, size, size);
   me_sepconvol(I1, I1, nx, ny, filter, filter, size, size);

   delete [] filter;
}


int me_median_compare(const void *i, const void *j)
{
	float *val1, *val2;

	val1 = (float *) i;
	val2 = (float *) j;

	if (*val1 < *val2)
		return -1;

	if (*val1 > *val2)
		return 1;

	return 0;
}


void me_median_filtering(float *in, int nx, int ny, int wsize)
{
	float median_vector[wsize], *out;
	int i, pixels, nrow, ncol, border;
	int x,y,xx,yy,xx0,yy0;

	border = wsize >> 1;
	out = new float[nx * ny];

	nrow = nx;
	ncol = ny;

	for (x=0;x<ncol;x++) {
		for (y=0;y<nrow;y++) {
			i = 0;

			for(yy = y - border; yy <= y + border; yy++) {
				for(xx = x - border; xx <= x + border; xx++) {
					xx0 = xx;
					yy0 = yy;

					/* Symmetry */

					if (xx0<0)
						xx0 = -xx0-1;

					if (xx0>=ncol)
						xx0 = 2*ncol-xx0-1;

					if (yy0<0)
						yy0 = -yy0-1;

					if (yy0>=nrow)
						yy0 = 2*nrow-yy0-1;

					/* Access data */

					median_vector[i++] = in[yy0*ncol+xx0];
				}
			}

			qsort(median_vector,i,sizeof(float),me_median_compare);
			out[y*ncol+x] = median_vector[i/2];
		}
	}

	pixels = nx *  ny;

	for(i = 0; i < pixels; i++) {
		in[i] = out[i];
	}

	delete [] out;
}



