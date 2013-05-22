/*
 * zach_TVL1_optical_flow.cpp
 *
 *  Created on: Jan 23, 2013
 *      Author: upf
 */

#include "headers/zach_TVL1_optical_flow.h"

/* public */

Zach_TVL1_OpticalFlow::Zach_TVL1_OpticalFlow(bool fake)
{
	_tau = 0.25;
	_lambda = 0.15;	//0.3
	_tita = 0.3;	//0.15
	_nscales = 5;
	_zoom_factor = 0.5;
	_nwarps = 10;
	_outit = 20;
	_innit = 20;
	_median = 1;
	_verbose = false;

	_watchdog = 0;
}


Zach_TVL1_OpticalFlow::Zach_TVL1_OpticalFlow(float tau, float lambda, float tita, int nscales, float zoom_factor,
						  	  	  	  	  	 int nwarps, int outit, int innit, int median, bool verbose)
{
	_tau = tau;
	_lambda = lambda;
	_tita = tita;
	_nscales = nscales;
	_zoom_factor = zoom_factor;
	_nwarps = nwarps;
	_outit = outit;
	_innit = innit;
	_median = median;
	_verbose = verbose;

	_watchdog = 0;
}


Zach_TVL1_OpticalFlow::Zach_TVL1_OpticalFlow(float tau, float lambda, float tita, int nwarps,
						  	  	  	  	  	 int outit, int innit, int median, bool verbose)
{
	_tau = tau;
	_lambda = lambda;
	_tita = tita;
	_nscales = 3;
	_zoom_factor = 0.5;
	_nwarps = nwarps;
	_outit = outit;
	_innit = innit;
	_median = median;
	_verbose = verbose;

	_watchdog = 0;
}


Zach_TVL1_OpticalFlow::~Zach_TVL1_OpticalFlow()
{

}


bool Zach_TVL1_OpticalFlow::calculate(float *I0, float *I1, float *u1, float *u2, const int nx, const int ny)
{
	const int   size = nx * ny;
	const float l_t = _lambda * _tita;

	float *I1x    = new float[size];
	float *I1y    = new float[size];
	float *I1w    = new float[size];
	float *I1wx   = new float[size];
	float *I1wy   = new float[size];
	float *rho_c  = new float[size];
	float *v1     = new float[size];
	float *v2     = new float[size];
	float *p11    = new float[size];
	float *p12    = new float[size];
	float *p21    = new float[size];
	float *p22    = new float[size];
	float *div    = new float[size];
	float *grad   = new float[size];
	float *div_p1 = new float[size];
	float *div_p2 = new float[size];
	float *u1x    = new float[size];
	float *u1y    = new float[size];
	float *u2x    = new float[size];
	float *u2y    = new float[size];

	FieldOperations::centered_gradient(I1, I1x, I1y, nx, ny);

	for(int warp = 0; warp < _nwarps; warp++) {
		//calculate the warpings of the target image and its derivatives
		Warping::warp(I1,  u1, u2, I1w,  nx, ny);
		Warping::warp(I1x, u1, u2, I1wx, nx, ny);
		Warping::warp(I1y, u1, u2, I1wy, nx, ny);

		for(int i = 0; i < size; i++) {
			//initialization of the variables
			v1[i]  = u1[i];
			v2[i]  = u2[i];
			const float Ix2 = I1wx[i] * I1wx[i];
			const float Iy2 = I1wy[i] * I1wy[i];

			//store the |Grad(I1)|^2
			grad[i]  = (Ix2 + Iy2);

			//compute the constant part of the rho function
			rho_c[i] = (I1w[i] - I1wx[i] * u1[i] - I1wy[i] * u2[i] - I0[i]);
		}

		for(int n_outer = 0; n_outer < _outit; n_outer++) {
			//estimate the values of the variable (v1, v2)
			for(int i = 0; i < size; i++) {
				const float rho = rho_c[i] + (I1wx[i] * u1[i] + I1wy[i] * u2[i]);

				float d1, d2;
				if(rho < - l_t * grad[i]) {
					d1 = l_t * I1wx[i];
					d2 = l_t * I1wy[i];
				} else {
					if(rho > l_t * grad[i]) {
						d1 = -l_t * I1wx[i];
						d2 = -l_t * I1wy[i];
					} else {
						if(grad[i] < 1E-10) {
							d1 = d2 = 0;
						} else {
							d1 = - rho * I1wx[i] / grad[i];
							d2 = - rho * I1wy[i] / grad[i];
						}
					}
				}

				v1[i] = u1[i] + d1;
				v2[i] = u2[i] + d2;

				u1[i] = v1[i];
				u2[i] = v2[i];
			}

			// initialize p to zero
			for(int i = 0; i < size; i++) {
				p11[i] = p12[i] = 0.0;
				p21[i] = p22[i] = 0.0;
			}

			for(int n_inner = 0; n_inner < _innit; n_inner++) {
				//calculate the gradient of the optical flow (u1, u2)
				FieldOperations::forward_gradient(u1, u1x, u1y, nx ,ny);
				FieldOperations::forward_gradient(u2, u2x, u2y, nx ,ny);

				//estimate the values of the dual variable (p1, p2)
				for(int i = 0; i < size; i++) {
					const float taut = _tau / _tita;
					const float g1   = sqrt(u1x[i] * u1x[i] + u1y[i] * u1y[i]);
					const float g2   = sqrt(u2x[i] * u2x[i] + u2y[i] * u2y[i]);
					const float ng1  = 1.0 + taut * g1;
					const float ng2  = 1.0 + taut * g2;

					p11[i] = (p11[i] + taut * u1x[i]) / ng1;
					p12[i] = (p12[i] + taut * u1y[i]) / ng1;
					p21[i] = (p21[i] + taut * u2x[i]) / ng2;
					p22[i] = (p22[i] + taut * u2y[i]) / ng2;
				}

				//calculate the divergence of the dual variable (p1, p2)
				FieldOperations::divergence(p11, p12, div_p1, nx ,ny);
				FieldOperations::divergence(p21, p22, div_p2, nx ,ny);

				//estimate the values of the intermediate optical flow (u1, u2)
				for(int i = 0; i < size; i++) {
					u1[i] = v1[i] + _tita * div_p1[i];
					u2[i] = v2[i] + _tita * div_p2[i];
				}
			}

			if (!can_continue())
				return false;	//TODO: free memory
		}

		if (_median > 1) {
			me_median_filtering(u1, nx, ny, _median);
			me_median_filtering(u2, nx, ny, _median);
		}

	}

	delete []I1x;
	delete []I1y;
	delete []I1w;
	delete []I1wx;
	delete []I1wy;
	delete []rho_c;
	delete []v1;
	delete []v2;
	delete []p11;
	delete []p12;
	delete []p21;
	delete []p22;
	delete []div;
	delete []grad;
	delete []div_p1;
	delete []div_p2;
	delete []u1x;
	delete []u1y;
	delete []u2x;
	delete []u2y;

	return true;
}


bool Zach_TVL1_OpticalFlow::calculate_with_multiscale(float *I0, float *I1, float *u1, float *u2, const int nxx, const int nyy)
{
	int size = nxx * nyy;

	std::vector<float *> I0s(_nscales);
	std::vector<float *> I1s(_nscales);
	std::vector<float *> u1s(_nscales);
	std::vector<float *> u2s(_nscales);

	std::vector<int> nx(_nscales);
	std::vector<int> ny(_nscales);

	I0s[0] = new float[size];
	I1s[0] = new float[size];

	u1s[0] = u1;
	u2s[0] = u2;
	nx [0] = nxx;
	ny [0] = nyy;

	//initialize the images and the flow at the finest scale
	for(int i = 0; i < size; i++) {
		I0s[0][i] = I0[i];
		I1s[0][i] = I1[i];
		u1s[0][i] = u2s[0][i] = 0.0;
	}

	//create the scales
	for(int s = 1; s < _nscales; s++) {
		CoarseToFine::zoom_size(nx[s-1], ny[s-1], nx[s], ny[s], _zoom_factor); //get x,y sizes for s-th scale
		const int sizes = nx[s] * ny[s];

		I0s[s] = new float[sizes];
		I1s[s] = new float[sizes];
		u1s[s] = new float[sizes];
		u2s[s] = new float[sizes];

		//zoom in the images to create the pyramidal structure
		CoarseToFine::zoom(I0s[s-1], I0s[s], nx[s-1], ny[s-1], _zoom_factor);
		CoarseToFine::zoom(I1s[s-1], I1s[s], nx[s-1], ny[s-1], _zoom_factor);
	}

	//initialize the flow at the coarsest scale
	int npix = nx[_nscales - 1] * ny[_nscales - 1];
	for(int i = 0; i < npix; i++) {
		u1s[_nscales - 1][i] = u2s[_nscales - 1][i] = 0.0;
	}

	// Calculate OF
	for(int s = _nscales-1; s >= 0; s--)
	{
		if(_verbose) std::cout << "Scale " << s << std::endl;

		bool success = calculate(I0s[s], I1s[s], u1s[s], u2s[s], nx[s], ny[s]);

		if (!success)
			return false;

		if(s) {
			CoarseToFine::zoom_out(u1s[s], u1s[s-1], nx[s], ny[s], nx[s-1], ny[s-1]);
			CoarseToFine::zoom_out(u2s[s], u2s[s-1], nx[s], ny[s], nx[s-1], ny[s-1]);

			int size = nx[s-1] * ny[s-1];

			for(int i = 0; i < size; i++) {
				u1s[s-1][i] *= (float) 1.0 / _zoom_factor;
				u2s[s-1][i] *= (float) 1.0 / _zoom_factor;
			}
		}
	}

	for(int i = 1; i < _nscales; i++) {
		delete []I0s[i];
		delete []I1s[i];
		delete []u1s[i];
		delete []u2s[i];
	}

	return true;
}


float Zach_TVL1_OpticalFlow::get_tau()
{
	return _tau;
}

void Zach_TVL1_OpticalFlow::set_tau(float tau)
{
	_tau = tau;
}

float Zach_TVL1_OpticalFlow::get_lambda()
{
	return _lambda;
}

void Zach_TVL1_OpticalFlow::set_lambda(float lambda)
{
	_lambda = lambda;
}

float Zach_TVL1_OpticalFlow::get_tita()
{
	return _tita;
}

void Zach_TVL1_OpticalFlow::set_tita(float tita)
{
	_tita = tita;
}

float Zach_TVL1_OpticalFlow::get_zoom_factor()
{
	return _zoom_factor;
}

void Zach_TVL1_OpticalFlow::set_zoom_factor(float zoom_factor)
{
	_zoom_factor = zoom_factor;
}

int Zach_TVL1_OpticalFlow::get_nscales()
{
	return _nscales;
}

void Zach_TVL1_OpticalFlow::set_nscales(int nscales)
{
	_nscales = nscales;
}

int Zach_TVL1_OpticalFlow::get_nwarps()
{
	return _nwarps;
}

void Zach_TVL1_OpticalFlow::set_nwarps(int nwarps)
{
	_nwarps = nwarps;
}

int Zach_TVL1_OpticalFlow::get_outit()
{
	return _outit;
}

void Zach_TVL1_OpticalFlow::set_outit(int outit)
{
	_outit = outit;
}

int Zach_TVL1_OpticalFlow::get_innit()
{
	return _innit;
}

void Zach_TVL1_OpticalFlow::set_innit(int innit)
{
	_innit = innit;
}

int Zach_TVL1_OpticalFlow::get_median()
{
	return _median;
}

void Zach_TVL1_OpticalFlow::set_median(int median)
{
	_median = median;
}

void Zach_TVL1_OpticalFlow::set_watchdog(IWatchdog *watchdog)
{
	_watchdog = watchdog;
}

bool Zach_TVL1_OpticalFlow::can_continue()
{
	if (!_watchdog)
		return true;

	return _watchdog->can_continue();
}





