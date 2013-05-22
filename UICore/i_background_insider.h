/*
 * i_background_insider.h
 *
 *  Created on: May 22, 2013
 *      Author: Vadim Fedorov
 */

#ifndef I_BACKGROUND_INSIDER_H_
#define I_BACKGROUND_INSIDER_H_

#include "i_data.h"
#include "signal_watchdog.h"

class IBackgroundInsider
{
public:
	virtual ~IBackgroundInsider() {}

	virtual IWatchdog* get_watchdog() = 0;
	virtual void submit_data_portion(IData* data) = 0;
	virtual void announce_completion() = 0;
};


#endif /* I_BACKGROUND_INSIDER_H_ */
