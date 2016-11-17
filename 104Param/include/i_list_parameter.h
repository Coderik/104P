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

#ifndef I_LIST_PARAMETER_H_
#define I_LIST_PARAMETER_H_

#include <vector>
#include "i_parameter.h"

class IListParameter : public IParameter
{
public:
	virtual ~IListParameter() {}

	virtual std::string get_display_name() = 0;
	virtual std::vector<std::string> get_items() = 0;
	virtual bool set(unsigned int item_id) = 0;
	virtual int get() = 0;
};


#endif /* I_LIST_PARAMETER_H_ */
