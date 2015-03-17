/*
 * fast_math.h
 *
 *  Created on: Feb 24, 2015
 *      Author: Vadim Fedorov
 */

#ifndef LUT_MATH_H_
#define LUT_MATH_H_

/**
 * @brief Look-Up Table math
 */
class LUT
{
public:
	static double exp(double x);

private:
	union float_long {
		double f;
		long long l;
	};
	static double ExpAdjustment[256];
};



#endif /* LUT_MATH_H_ */
