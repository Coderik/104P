/*
 * i_parameter.h
 *
 *  Created on: Sep 11, 2013
 *      Author: Vadim Fedorov
 */

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
