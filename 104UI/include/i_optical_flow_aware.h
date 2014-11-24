/*
 * i_optical_flow_aware.h
 *
 *  Created on: Feb 17, 2014
 *      Author: Vadim Fedorov
 */

#ifndef I_OPTICAL_FLOW_AWARE_H_
#define I_OPTICAL_FLOW_AWARE_H_

class IOpticalFlowAware
{
public:
	virtual ~IOpticalFlowAware() {}
	virtual void optical_flow_changed() = 0;
};





#endif /* I_OPTICAL_FLOW_AWARE_H_ */
