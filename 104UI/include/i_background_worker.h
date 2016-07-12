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

#ifndef I_BACKGROUND_WORKER_H_
#define I_BACKGROUND_WORKER_H_

#include <sigc++/sigc++.h>
#include "i_background_insider.h"

#include "i_data.h"

class IBackgroundWorker
{
public:
	virtual ~IBackgroundWorker() {}

	virtual void start(sigc::slot1<void,IBackgroundInsider* > working_function) = 0;
	virtual void cancel() = 0;
	virtual void wait() = 0;

	typedef sigc::signal<void> type_signal_work_finished;
	virtual type_signal_work_finished signal_work_finished() = 0;

	typedef sigc::signal<void, IData* > type_signal_data_prepared;
	virtual type_signal_data_prepared signal_data_prepared() = 0;
};


#endif /* I_BACKGROUND_WORKER_H_ */
