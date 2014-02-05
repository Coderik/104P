/*
 * i_mask_aware.h
 *
 *  Created on: Feb 5, 2014
 *      Author: upf
 */

#ifndef I_MASK_AWARE_H_
#define I_MASK_AWARE_H_

class IMaskAware
{
public:
	virtual ~IMaskAware() {}
	virtual void mask_changed() = 0;
};



#endif /* I_MASK_AWARE_H_ */
