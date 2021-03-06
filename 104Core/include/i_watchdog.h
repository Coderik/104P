/**
 * Copyright (C) 2016, Vadim Fedorov <coderiks@gmail.com>
 *
 * This program is free software: you can use, modify and/or
 * redistribute it under the terms of the simplified BSD
 * License. You should have received a copy of this license along
 * this program. If not, see
 * <http://www.opensource.org/licenses/bsd-license.html>.
 */

/// Created on: Jan 29, 2013

#ifndef I_WATCHDOG_H_
#define I_WATCHDOG_H_

class IWatchdog
{
public:
	virtual ~IWatchdog() {};
	virtual bool can_continue() = 0;
};


#endif /* I_WATCHDOG_H_ */
