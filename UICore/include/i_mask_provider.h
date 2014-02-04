/*
 * i_mask_provider.h
 *
 *  Created on: Feb 4, 2014
 *      Author: Vadim Fedorov
 */

#ifndef I_MASK_PROVIDER_H_
#define I_MASK_PROVIDER_H_

#include "sequence_mask.h"

class IMaskProvider
{
public:
	virtual ~IMaskProvider() {}

	virtual SequenceMask* request_mask() = 0;
};



#endif /* I_MASK_PROVIDER_H_ */
