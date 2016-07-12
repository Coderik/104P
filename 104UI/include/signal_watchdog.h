/**
 * Copyright (C) 2016, Vadim Fedorov <coderiks@gmail.com>
 *
 * This program is free software: you can use, modify and/or
 * redistribute it under the terms of the simplified BSD
 * License. You should have received a copy of this license along
 * this program. If not, see
 * <http://www.opensource.org/licenses/bsd-license.html>.
 */

/// Created on: Jan 30, 2013

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
