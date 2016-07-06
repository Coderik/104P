/**
 * Copyright (C) 2016, Vadim Fedorov <coderiks@gmail.com>
 *
 * This program is free software: you can use, modify and/or
 * redistribute it under the terms of the simplified BSD
 * License. You should have received a copy of this license along
 * this program. If not, see
 * <http://www.opensource.org/licenses/bsd-license.html>.
 */

/// Created on: Apr 16, 2013

#ifndef I_RIG_MANAGER_H_
#define I_RIG_MANAGER_H_

#include <string>
#include "rig.h"

class IRigManager
{
public:
	virtual ~IRigManager() {};

	virtual void add_rig(Rig* rig, std::string display_name) = 0;
	virtual void initialize_rigs() = 0;
};


#endif /* I_RIG_MANAGER_H_ */
