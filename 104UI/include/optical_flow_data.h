/**
 * Copyright (C) 2016, Vadim Fedorov <coderiks@gmail.com>
 *
 * This program is free software: you can use, modify and/or
 * redistribute it under the terms of the simplified BSD
 * License. You should have received a copy of this license along
 * this program. If not, see
 * <http://www.opensource.org/licenses/bsd-license.html>.
 */

/// Created on: May 22, 2013

#ifndef OPTICAL_FLOW_DATA_H_
#define OPTICAL_FLOW_DATA_H_

#include "i_data.h"

struct OpticalFlowData : public IData
{
public:
	virtual ~OpticalFlowData() {}

	float* flow_x;
	float* flow_y;
	int size_x;
	int size_y;
	int index;
};

#endif /* OPTICAL_FLOW_DATA_H_ */
