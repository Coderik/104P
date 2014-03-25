/*
 * signal_watchdog.h
 *
 *  Created on: Jan 30, 2013
 *      Author: Vadim Fedorov
 */

#ifndef SIGNAL_WATCHDOG_H_
#define SIGNAL_WATCHDOG_H_

#include <sigc++/sigc++.h>

#include "i_watchdog.h"

class SignalWatchdog : public IWatchdog
{
public:
	virtual ~SignalWatchdog() {}

	virtual bool can_continue();

	typedef sigc::signal<bool> type_signal_ask_permission;
	type_signal_ask_permission signal_ask_permission()
	{
		return _signal_ask_permission;
	}

private:
	type_signal_ask_permission _signal_ask_permission;
};


#endif /* SIGNAL_WATCHDOG_H_ */
