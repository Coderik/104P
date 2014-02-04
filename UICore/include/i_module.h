/*
 * i_module.h
 *
 *  Created on: Feb 4, 2014
 *      Author: Vadim Fedorov
 */

#ifndef I_MODULE_H_
#define I_MODULE_H_

#include "i_modulable.h"

class IModule
{
public:
	virtual ~IModule() {}

	virtual void initialize(IModulable *modulable) = 0;
};



#endif /* I_MODULE_H_ */
