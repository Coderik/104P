/**
 * Copyright (C) 2016, Vadim Fedorov <coderiks@gmail.com>
 *
 * This program is free software: you can use, modify and/or
 * redistribute it under the terms of the simplified BSD
 * License. You should have received a copy of this license along
 * this program. If not, see
 * <http://www.opensource.org/licenses/bsd-license.html>.
 */

/// Created on: May 22, 2013

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
