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

#ifndef I_MODULE_H_
#define I_MODULE_H_

#include "i_modulable.h"

class IModule
{
public:
	virtual ~IModule() {}

	virtual void initialize(IModulable *modulable) = 0;

	// TODO: notify module about rig change
};



#endif /* I_MODULE_H_ */
