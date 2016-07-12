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

#ifndef I_MASK_PROVIDER_H_
#define I_MASK_PROVIDER_H_

#include "mask_sequence.h"
#include "i_mask_aware.h"

class IMaskProvider
{
public:
	virtual ~IMaskProvider() {}

	virtual MaskSequenceFx request_mask() = 0;
};



#endif /* I_MASK_PROVIDER_H_ */
