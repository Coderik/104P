/*
 * i_watchdog.h
 *
 *  Created on: Jan 29, 2013
 *      Author: upf
 */

#ifndef I_WATCHDOG_H_
#define I_WATCHDOG_H_

class IWatchdog
{
public:
	virtual ~IWatchdog() {};
	virtual bool can_continue() = 0;
};


#endif /* I_WATCHDOG_H_ */
