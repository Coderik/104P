/*
 * i_module_manager.h
 *
 *  Created on: Feb 4, 2014
 *      Author: Vadim Fedorov
 */

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
