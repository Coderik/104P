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

#ifndef I_PARAMETER_H_
#define I_PARAMETER_H_

#include <string>

using namespace std;

class IParameter
{
public:
	virtual ~IParameter() {}

	virtual string get_display_name() = 0;
};


#endif /* I_PARAMETER_H_ */
