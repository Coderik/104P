/*
 * intensity_code.h
 *
 *  Created on: Feb 8, 2013
 *      Author: Vadim Fedorov
 */

#ifndef INTENSITY_CODE_H_
#define INTENSITY_CODE_H_

#include <algorithm>
#include <math.h>

#include "i_optical_flow_code.h"

class IntensityCode : public IOptivalFlowCode
{
public:
	IntensityCode();
	virtual ~IntensityCode();

	virtual void get_color(float fx, float fy, char *color);
};


#endif /* INTENSITY_CODE_H_ */
