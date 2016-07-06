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

#include "warping.h"

void Warping::warp(const float *input, const float *u, const float *v,
					 float *output, const int nx, const int ny)
{
	#pragma omp parallel
	{
		#pragma omp for schedule(dynamic) nowait
		for(int j = 0; j < ny; j++) {
			for(int i = 0; i < nx; i++) {
				const float uu = (float) (i + u[i + nx * j]);
				const float vv = (float) (j + v[i + nx * j]);

				if ((uu < 0) || (uu > (nx - 1)) || (vv < 0) || (vv > (ny - 1))) {
					output[i + nx * j] = 0;
				} else {
					output[i + nx * j] = Interpolation::bicubic(input, nx, ny, uu, vv);
				}
			}
		}
	}
}

