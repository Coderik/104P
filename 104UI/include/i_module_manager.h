/**
 * Copyright (C) 2016, Vadim Fedorov <coderiks@gmail.com>
 *
 * This program is free software: you can use, modify and/or
 * redistribute it under the terms of the simplified BSD
 * License. You should have received a copy of this license along
 * this program. If not, see
 * <http://www.opensource.org/licenses/bsd-license.html>.
 */

/// Created on: Feb 4, 2014

#ifndef I_MODULE_MANAGER_H_
#define I_MODULE_MANAGER_H_

#include "i_module.h"

class IModuleManager
{
public:
	virtual ~IModuleManager() {}

	virtual void add_module(IModule *module) = 0;
};



#endif /* I_MODULE_MANAGER_H_ */
