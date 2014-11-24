/*
 * i_list_parameter.h
 *
 *  Created on: Sep 11, 2013
 *      Author: Vadim Fedorov
 */

#ifndef I_LIST_PARAMETER_H_
#define I_LIST_PARAMETER_H_

#include <vector>
#include "i_parameter.h"

using namespace std;

class IListParameter : public IParameter
{
public:
	virtual ~IListParameter() {}

	virtual string get_display_name() = 0;
	virtual vector<string> get_items() = 0;
	virtual bool set(unsigned int item_id) = 0;
	virtual int get() = 0;
};


#endif /* I_LIST_PARAMETER_H_ */
