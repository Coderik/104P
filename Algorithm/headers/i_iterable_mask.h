/*
 * i_iterable_mask.h
 *
 *  Created on: Apr 21, 2013
 *      Author: Vadim Fedorov
 */

#ifndef I_ITERABLE_MASK_H_
#define I_ITERABLE_MASK_H_

#include "point.h"

class IIterableMask
{
public:
	virtual ~IIterableMask() {}

	virtual Point first() = 0;
	virtual Point last() = 0;
	virtual Point next(const Point current) = 0;
	virtual Point prev(const Point current) = 0;
};


#endif /* I_ITERABLE_MASK_H_ */