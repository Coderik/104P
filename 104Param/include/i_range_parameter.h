/**
 * Copyright (C) 2016, Vadim Fedorov <coderiks@gmail.com>
 *
 * This program is free software: you can use, modify and/or
 * redistribute it under the terms of the simplified BSD
 * License. You should have received a copy of this license along
 * this program. If not, see
 * <http://www.opensource.org/licenses/bsd-license.html>.
 */

/// Created on: Sep 11, 2013

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
