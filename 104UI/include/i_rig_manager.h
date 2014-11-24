/*
 * i_rig_manager.h
 *
 *  Created on: Apr 16, 2013
 *      Author: Vadim Fedorov
 */

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
