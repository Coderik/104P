/*
 * field_operations.cpp
 *
 *  Created on: Jan 23, 2013
 *      Author: upf
 */

#include <omp.h>

#include "field_operations.h"

void FieldOperations::divergence(const float *v1, const float *v2, float *div, const int nx, const int ny)
{
	//apply the divergence to the center body of the image
	for(int i = 1; i < ny-1; i++) {
		for(int j = 1; j < nx-1; j++) {
			const int p  = i * nx + j;
			const int p1 = p - 1;
			const int p2 = p - nx;

			const float v1x = v1[p] - v1[p1];
			const float v2y = v2[p] - v2[p2];

			div[p] = v1x + v2y;
		}
	}

	//apply the divergence to the first and last rows
	for(int j = 1; j < nx-1; j++) {
		const int p = (ny-1) * nx + j;

		div[j] = v1[j] - v1[j-1] + v2[j];
		div[p] = v1[p] - v1[p-1] - v2[p-nx];
	}

	//apply the divergence to the first and last columns
	for(int i = 1; i < ny-1; i++) {
		const int p1 = i * nx;
		const int p2 = (i+1) * nx - 1;

		div[p1] =  v1[p1]   + v2[p1] - v2[p1 - nx];
		div[p2] = -v1[p2-1] + v2[p2] - v2[p2 - nx];
	}

	div[0]         =  v1[0] + v2[0];
	div[nx-1]      = -v1[nx - 2] + v2[nx - 1];
	div[(ny-1)*nx] =  v1[(ny-1)*nx] - v2[(ny-2)*nx];
	div[ny*nx-1]   = -v1[ny*nx - 2] - v2[(ny-1)*nx - 1];
}


void FieldOperations::forward_gradient(const float *f, float *fx, float *fy, const int nx, const int ny)
{
	#pragma omp parallel
	{
		//apply the gradient to the center body of the image
		#pragma omp for schedule(dynamic) nowait
		for(int i = 0; i < ny-1; i++) {
			for(int j = 0; j < nx-1; j++) {
				const int p  = i * nx + j;
				const int p1 = p + 1;
				const int p2 = p + nx;

				fx[p] = f[p1] - f[p];
				fy[p] = f[p2] - f[p];
			}
		}

		//apply the gradient to the last row
		#pragma omp for schedule(dynamic) nowait
		for(int j = 0; j < nx-1; j++) {
			const int p = (ny-1) * nx + j;

			fx[p] = f[p+1] - f[p];
			fy[p] = 0;
		}

		//apply the gradient to the last column
		#pragma omp for schedule(dynamic) nowait
		for(int i = 1; i < ny; i++) {
			const int p = i * nx-1;

			fx[p] = 0;
			fy[p] = f[p+nx] - f[p];
		}

		fx[ny * nx - 1] = 0;
		fy[ny * nx - 1] = 0;
	}
}


void FieldOperations::centered_gradient(const float *in, float *dx, float *dy, const int nx, const int ny)
{
	float filter_der[5] = {-1.0/12.0, 8.0/12.0, 0.0, -8.0/12.0, 1.0/12.0};
	float filter_id[1]  = {1.0};

	Filtering::separate_convolution(in, dx, nx, ny, filter_der, filter_id, 5, 1);
	Filtering::separate_convolution(in, dy, nx, ny, filter_id, filter_der, 1, 5);
}


void FieldOperations::centered_temporal_gradient(const float *prev_frame, const float *next_frame, float *dt, const int length)
{
	for (int i = 0; i < length; ++i) {
		dt[i] = next_frame[i] - prev_frame[i];
	}
}


void FieldOperations::centered_convective_derivative(const float *prev_frame, const float *next_frame,
													 const float *bwd_flow, const float *fwd_flow,
													 float *result, const int nx, const int ny)
{
	if (bwd_flow && fwd_flow) {
		for (int y = 0; y < ny; ++y) {
			for (int x = 0; x < nx; ++x) {
				int index = y * nx + x;

				float bwd_x = (float) x + bwd_flow[index * 2];
				float bwd_y = (float) y + bwd_flow[index * 2 + 1];
				float fwd_x = (float) x + fwd_flow[index * 2];
				float fwd_y = (float) y + fwd_flow[index * 2 + 1];
				bwd_x = std::max(std::min(bwd_x, (float)nx - 1), 0.0f);
				bwd_y = std::max(std::min(bwd_y, (float)ny - 1), 0.0f);
				fwd_x = std::max(std::min(fwd_x, (float)nx - 1), 0.0f);
				fwd_y = std::max(std::min(fwd_y, (float)ny - 1), 0.0f);

				// TODO: use bilinear instead of nearest interpolation here
				int bwd_index = ((int)(bwd_y + 0.5f)) * nx + (int)(bwd_x + 0.5f);
				int fwd_index = ((int)(fwd_y + 0.5f)) * nx + (int)(fwd_x + 0.5f);
				float bwd_color = prev_frame[bwd_index];
				float fwd_color = next_frame[fwd_index];
				result[index] = fwd_color - bwd_color;
			}
		}
	} else if (bwd_flow) {
		for (int y = 0; y < ny; ++y) {
			for (int x = 0; x < nx; ++x) {
				int index = y * nx + x;

				float bwd_x = (float) x + bwd_flow[index * 2];
				float bwd_y = (float) y + bwd_flow[index * 2 + 1];
				bwd_x = std::max(std::min(bwd_x, (float)nx - 1), 0.0f);
				bwd_y = std::max(std::min(bwd_y, (float)ny - 1), 0.0f);

				// TODO: use bilinear instead of nearest interpolation here
				int bwd_index = ((int)(bwd_y + 0.5f)) * nx + (int)(bwd_x + 0.5f);
				float bwd_color = prev_frame[bwd_index];
				float fwd_color = next_frame[index];
				result[index] = fwd_color - bwd_color;
			}
		}
	} else if (fwd_flow) {
		for (int y = 0; y < ny; ++y) {
			for (int x = 0; x < nx; ++x) {
				int index = y * nx + x;

				float fwd_x = (float) x + fwd_flow[index * 2];
				float fwd_y = (float) y + fwd_flow[index * 2 + 1];
				fwd_x = std::max(std::min(fwd_x, (float)nx - 1), 0.0f);
				fwd_y = std::max(std::min(fwd_y, (float)ny - 1), 0.0f);

				// TODO: use bilinear instead of nearest interpolation here
				int fwd_index = ((int)(fwd_y + 0.5f)) * nx + (int)(fwd_x + 0.5f);
				float bwd_color = prev_frame[index];
				float fwd_color = next_frame[fwd_index];
				result[index] = fwd_color - bwd_color;
			}
		}
	} else {
		for (int i = 0; i < nx * ny; ++i) {
			result[i] = 0.0f;
		}
	}
}




