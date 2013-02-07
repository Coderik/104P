/*
 * signal_watchdog.cpp
 *
 *  Created on: Jan 30, 2013
 *      Author: Vadim Fedorov
 */


#include "signal_watchdog.h"

bool SignalWatchdog::can_continue()
{
	if (_signal_ask_permission.empty())
		return true;

	return _signal_ask_permission.emit();
}

