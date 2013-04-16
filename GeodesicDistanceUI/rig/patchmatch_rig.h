/*
 * patchmatch_rig.h
 *
 *  Created on: Apr 15, 2013
 *      Author: Vadim Fedorov
 */

#ifndef PATCHMATCH_RIG_H_
#define PATCHMATCH_RIG_H_

#include "rig.h"

class PatchMatch_Rig : public Rig
{
public:
	PatchMatch_Rig();
	virtual ~PatchMatch_Rig() {};

	virtual void initialize(IHull *hull);
};




#endif /* PATCHMATCH_RIG_H_ */
