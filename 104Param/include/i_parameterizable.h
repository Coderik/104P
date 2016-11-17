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

#ifndef I_PARAMETERIZABLE_H_
#define I_PARAMETERIZABLE_H_

#include <vector>
#include "i_parameter.h"

class IParameterizable
{
public:
	virtual ~IParameterizable() {}

	virtual std::vector<IParameter *> get_parameters() = 0;
};


#endif /* I_PARAMETERIZABLE_H_ */
