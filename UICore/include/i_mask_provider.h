/*
 * i_mask_provider.h
 *
 *  Created on: Feb 4, 2014
 *      Author: Vadim Fedorov
 */

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
