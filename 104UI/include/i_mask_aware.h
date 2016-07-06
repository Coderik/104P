/**
 * Copyright (C) 2016, Vadim Fedorov <coderiks@gmail.com>
 *
 * This program is free software: you can use, modify and/or
 * redistribute it under the terms of the simplified BSD
 * License. You should have received a copy of this license along
 * this program. If not, see
 * <http://www.opensource.org/licenses/bsd-license.html>.
 */

/// Created on: Feb 5, 2014

#ifndef I_MASK_AWARE_H_
#define I_MASK_AWARE_H_

#include "mask_group.h"

class IMaskAware
{
public:
	virtual ~IMaskAware() {}
	virtual MaskGroup* get_mask_group() = 0;
};



#endif /* I_MASK_AWARE_H_ */
