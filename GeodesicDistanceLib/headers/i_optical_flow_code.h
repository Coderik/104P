/*
 * i_optical_flow_code.h
 *
 *  Created on: Feb 8, 2013
 *      Author: Vadim Fedorov
 */

#ifndef I_OPTICAL_FLOW_CODE_H_
#define I_OPTICAL_FLOW_CODE_H_

class IOptivalFlowCode
{
public:
	virtual ~IOptivalFlowCode() {};
	virtual void get_color(float fx, float fy, char *color) = 0;
};


#endif /* I_OPTICAL_FLOW_CODE_H_ */
