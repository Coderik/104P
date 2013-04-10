/*
 * i_rig.h
 *
 *  Created on: Apr 10, 2013
 *      Author: Vadim Fedorv
 */

#ifndef I_RIG_H_
#define I_RIG_H_

class IRig
{
public:
	virtual ~IRig() {};
	virtual void activate() = 0;
	virtual void deactivate() = 0;
};


#endif /* I_RIG_H_ */
