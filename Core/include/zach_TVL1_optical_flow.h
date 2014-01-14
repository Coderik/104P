/*
 * zach_TVL1_optical_flow.h
 *
 *  Created on: Jan 23, 2013
 *      Author: upf
 */

#ifndef ZACH_TVL1_OPTICAL_FLOW_H_
#define ZACH_TVL1_OPTICAL_FLOW_H_

#include <iostream>
#include <vector>

#include "field_operations.h"
#include "filtering.h"
#include "sampling.h"
#include "warping.h"
#include "i_watchdog.h"

/**
  * Implementation of the Zach, Pock and Bischof dual TV-L1 optic flow method
  *
  * see reference:
  *  [1] C. Zach, T. Pock and H. Bischof, "A Duality Based Approach for Realtime
  *      TV-L1 Optical Flow", In Proceedings of Pattern Recognition (DAGM),
  *      Heidelberg, Germany, pp. 214-223, 2007
  *
  *
  * Details on the total variation minimization scheme can be found in:
  *  [2] A. Chambolle, "An Algorithm for Total Variation Minimization and
  *      Applications", Journal of Mathematical Imaging and Vision, 20: 89-97, 2004
**/

class Zach_TVL1_OpticalFlow
{
public:
	Zach_TVL1_OpticalFlow(bool fake);
	Zach_TVL1_OpticalFlow(float tau, float lambda, float tita, int nscales, float zoom_factor,
						  int nwarps, int outit, int innit, int median, bool verbose);
	Zach_TVL1_OpticalFlow(float tau, float lambda, float tita, int nwarps,
						  int outit, int innit, int median, bool verbose);
	~Zach_TVL1_OpticalFlow();

	bool calculate(float *I0, float *I1, float *u1, float *u2, const int nx, const int ny);
	bool calculate_with_multiscale(float *I0, float *I1, float *u1, float *u2, const int nxx, const int nyy);

	/* getters, setters */
	float get_tau();
	void set_tau(float tau);
	float get_lambda();
	void set_lambda(float lambda);
	float get_tita();
	void set_tita(float tita);
	float get_zoom_factor();
	void set_zoom_factor(float zoom_factor);
	int get_nscales();
	void set_nscales(int nscales);
	int get_nwarps();
	void set_nwarps(int nwarps);
	int get_outit();
	void set_outit(int outit);
	int get_innit();
	void set_innit(int innit);
	int get_median();
	void set_median(int median);

	void set_watchdog(IWatchdog *watchdog);

private:
	float _tau, _lambda, _tita;
	float _zoom_factor;
	int _nscales, _nwarps, _outit, _innit, _median;
	bool _verbose;
	IWatchdog *_watchdog;

	bool can_continue();
};


#endif /* ZACH_TVL1_OPTICAL_FLOW_H_ */
