/*
 * fast_math.h
 *
 *  Created on: Feb 24, 2015
 *      Author: Vadim Fedorov
 */

#ifndef FAST_MATH_H_
#define FAST_MATH_H_

class FastMath
{
public:
	static double lut_exp(double x);

private:
	union float_long {
		double f;
		long long l;
	};
	static double ExpAdjustment[256];
};



#endif /* FAST_MATH_H_ */
