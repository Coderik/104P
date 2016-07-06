/**
 * Copyright (C) 2016, Vadim Fedorov <coderiks@gmail.com>
 *
 * This program is free software: you can use, modify and/or
 * redistribute it under the terms of the simplified BSD
 * License. You should have received a copy of this license along
 * this program. If not, see
 * <http://www.opensource.org/licenses/bsd-license.html>.
 */

/// Created on: May 21, 2013

#ifndef BACKGROUND_WORKER_H_
#define BACKGROUND_WORKER_H_

#include <glibmm/threads.h>
#include <glibmm/dispatcher.h>

#include "i_background_worker.h"
#include "i_background_insider.h"

// Note: if there are several different works to compute in background, separate backgroung_worker might be used for each

class BackgroundWorker : public IBackgroundWorker, public IBackgroundInsider
{
public:
	BackgroundWorker();
	virtual ~BackgroundWorker();

	/* IBackgroundWorker members (UI thread side) */
	virtual void start(sigc::slot1<void,IBackgroundInsider* > working_function);
	virtual void cancel();
	virtual void wait();

	typedef sigc::signal<void> type_signal_work_finished;
	virtual type_signal_work_finished signal_work_finished()
	{
		return _signal_work_finished;
	}

	typedef sigc::signal<void, IData* > type_signal_data_prepared;
	virtual type_signal_data_prepared signal_data_prepared()
	{
		return _signal_data_prepared;
	}

	/* IBackgroundInsider members (Background thread side) */
	virtual IWatchdog* get_watchdog();
	virtual void submit_data_portion(IData* data);
	virtual void announce_completion();

private:
	type_signal_work_finished _signal_work_finished;
	type_signal_data_prepared _signal_data_prepared;
	Glib::Threads::Thread *_thread;
	Glib::Dispatcher _work_finish_dispatcher;
	Glib::Dispatcher _portial_result_dispatcher;
	IWatchdog* _watchdog;

	Glib::Threads::Mutex _background_work_mutex;	// covers following variables
	bool _aux_stop_background_work_flag;
	IData *_aux_partial_data;

	bool allow_background_computation();
	void pass_data_portion();
	void finish();
};




#endif /* BACKGROUND_WORKER_H_ */
