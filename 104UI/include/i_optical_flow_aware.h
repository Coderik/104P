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

#ifndef I_OPTICAL_FLOW_AWARE_H_
#define I_OPTICAL_FLOW_AWARE_H_

class IOpticalFlowAware
{
public:
	virtual ~IOpticalFlowAware() {}
	virtual void optical_flow_changed() = 0;
};





#endif /* I_OPTICAL_FLOW_AWARE_H_ */
