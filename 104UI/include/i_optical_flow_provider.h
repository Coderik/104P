/**
 * Copyright (C) 2016, Vadim Fedorov <coderiks@gmail.com>
 *
 * This program is free software: you can use, modify and/or
 * redistribute it under the terms of the simplified BSD
 * License. You should have received a copy of this license along
 * this program. If not, see
 * <http://www.opensource.org/licenses/bsd-license.html>.
 */

/// Created on: Feb 17, 2014

#ifndef I_OPTICAL_FLOW_PROVIDER_H_
#define I_OPTICAL_FLOW_PROVIDER_H_

#include <vector>
#include "image.h"

class IOpticalFlowProvider
{
public:
	virtual ~IOpticalFlowProvider() {}

	virtual std::vector<Image<float> > request_forward_optical_flow() = 0;
	virtual std::vector<Image<float> > request_backward_optical_flow() = 0;
	virtual bool request_has_optical_flow_data() = 0;
};



#endif /* I_OPTICAL_FLOW_PROVIDER_H_ */
