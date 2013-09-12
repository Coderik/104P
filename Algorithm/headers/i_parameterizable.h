/*
 * i_parameterizable.h
 *
 *  Created on: Sep 11, 2013
 *      Author: Vadim Fedorov
 */

#ifndef I_PARAMETERIZABLE_H_
#define I_PARAMETERIZABLE_H_

#include <vector>
#include "i_parameter.h"

class IParameterizable
{
public:
	virtual ~IParameterizable() {}

	virtual vector<IParameter *> get_parameters() = 0;
};


#endif /* I_PARAMETERIZABLE_H_ */
