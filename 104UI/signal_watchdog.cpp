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

#include "signal_watchdog.h"

bool SignalWatchdog::can_continue()
{
	if (_signal_ask_permission.empty())
		return true;

	return _signal_ask_permission.emit();
}

