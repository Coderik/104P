/**
 * Copyright (C) 2016, Vadim Fedorov <coderiks@gmail.com>
 *
 * This program is free software: you can use, modify and/or
 * redistribute it under the terms of the simplified BSD
 * License. You should have received a copy of this license along
 * this program. If not, see
 * <http://www.opensource.org/licenses/bsd-license.html>.
 */

/// Created on: Feb 8, 2013

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
