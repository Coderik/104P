/*
 * i_proxy.h
 *
 *  Created on: Feb 4, 2014
 *      Author: Vadim Fedorov
 */

#ifndef I_PROXY_H_
#define I_PROXY_H_

class IProxy
{
public:
	virtual ~IProxy() {}

	virtual void enable(bool should_enable = true) = 0;
	virtual void disable() = 0;
};



#endif /* I_PROXY_H_ */
