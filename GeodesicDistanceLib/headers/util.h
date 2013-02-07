/*
 * util.h
 *
 *  Created on: Jan 23, 2013
 *      Author: upf
 */

#ifndef UTIL_H_GEODIST
#define UTIL_H_GEODIST

#include <iostream>
#include <math.h>
#include <stdlib.h>

/**
 * This is a mixture of methods, which I do not know where to place yet.
 */

extern float *me_sgauss(float std, int n);

extern void me_sepconvol(float *in, float *out, int nx, int ny, float *filter_x, float *filter_y, int size_x, int size_y);

extern void me_prefilter(float *I0, float *I1, int nx, int ny);

extern int me_median_compare(const void *i, const void *j);

extern void me_median_filtering(float *in, int nx, int ny, int wsize);


#endif /* UTIL_H_ */
