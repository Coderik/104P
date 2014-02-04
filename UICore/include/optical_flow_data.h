/*
 * optical_flow_data.h
 *
 *  Created on: May 22, 2013
 *      Author: Vadim Fedorov
 */

#ifndef OPTICAL_FLOW_DATA_H_
#define OPTICAL_FLOW_DATA_H_

#include "i_data.h"

struct OpticalFlowData : public IData
{
public:
	virtual ~OpticalFlowData() {}

	float* flow_x;
	float* flow_y;
	int index;
};

#endif /* OPTICAL_FLOW_DATA_H_ */
