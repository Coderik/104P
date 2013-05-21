/*
 * background_worker.h
 *
 *  Created on: May 21, 2013
 *      Author: Vadim Fedorov
 */

#ifndef BACKGROUND_WORKER_H_
#define BACKGROUND_WORKER_H_

#include <glibmm/threads.h>
#include <glibmm/dispatcher.h>
#include <sigc++/sigc++.h>

#include "i_data.h"
#include "signal_watchdog.h"

class Background_Worker
{
public:
	Background_Worker();

	void start(sigc::slot1<void,Background_Worker> working_function);
	void cancel();

	IWatchdog* get_watchdog();
	void submit_data_portion(I_Data* data);
	void announce_completion();

	typedef sigc::signal<void> type_signal_work_finished;
	type_signal_work_finished signal_work_finished()
	{
		return _signal_work_finished;
	}

	typedef sigc::signal<void, I_Data* > type_signal_data_prepared;
	type_signal_data_prepared signal_data_prepared()
	{
		return _signal_data_prepared;
	}

private:
	type_signal_work_finished _signal_work_finished;
	type_signal_data_prepared _signal_data_prepared;
	Glib::Threads::Thread *_thread;
	Glib::Dispatcher _work_finish_dispatcher;
	Glib::Dispatcher _portial_result_dispatcher;
	IWatchdog* _watchdog;

	Glib::Threads::Mutex _background_work_mutex;	// covers following variables
	bool _aux_stop_background_work_flag;
	I_Data *_aux_partial_data;

	bool allow_background_computation();
	void pass_data_portion();
	void finish();
};




#endif /* BACKGROUND_WORKER_H_ */
