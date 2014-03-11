/*
 * i_mask_aware.h
 *
 *  Created on: Feb 5, 2014
 *      Author: upf
 */

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
