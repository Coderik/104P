/*
 * i_optical_flow_provider.h
 *
 *  Created on: Feb 17, 2014
 *      Author: Vadim Fedorov
 */

#ifndef I_OPTICAL_FLOW_PROVIDER_H_
#define I_OPTICAL_FLOW_PROVIDER_H_

#include <vector>
#include "optical_flow_container.h"

using namespace std;

class IOpticalFlowProvider
{
public:
	virtual ~IOpticalFlowProvider() {}

	virtual vector<OpticalFlowContainer*> request_forward_optical_flow() = 0;
	virtual vector<OpticalFlowContainer*> request_backward_optical_flow() = 0;
	virtual bool request_has_optical_flow_data() = 0;
};



#endif /* I_OPTICAL_FLOW_PROVIDER_H_ */
