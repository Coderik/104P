/**
 * Copyright (C) 2016, Vadim Fedorov <coderiks@gmail.com>
 *
 * This program is free software: you can use, modify and/or
 * redistribute it under the terms of the simplified BSD
 * License. You should have received a copy of this license along
 * this program. If not, see
 * <http://www.opensource.org/licenses/bsd-license.html>.
 */

/// Created on: Apr 21, 2013

#ifndef I_ITERABLE_MASK_H_
#define I_ITERABLE_MASK_H_

#include "point.h"

class IIterableMask
{
public:
	virtual ~IIterableMask() {}

	virtual Point first() const = 0;
	virtual Point last() const = 0;
	virtual Point next(const Point &current) const = 0;
	virtual Point prev(const Point &current) const = 0;
};


#endif /* I_ITERABLE_MASK_H_ */
