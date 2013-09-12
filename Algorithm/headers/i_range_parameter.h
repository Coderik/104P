/*
 * i_range_parameter.h
 *
 *  Created on: Sep 11, 2013
 *      Author: Vadim Fedorov
 */

#ifndef I_RANGE_PARAMETER_H_
#define I_RANGE_PARAMETER_H_

#include "i_parameter.h"

class IRangeParameter : public IParameter
{
public:
	virtual ~IRangeParameter() {}
	virtual string get_display_name() = 0;
	virtual float get_lower_boundary() = 0;
	virtual float get_upper_boundary() = 0;
	virtual int get_digits() = 0;
	virtual bool set(float value) = 0;
	virtual float get() = 0;
};


#endif /* I_RANGE_PARAMETER_H_ */
