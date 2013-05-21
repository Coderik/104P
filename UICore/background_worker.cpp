/*
 * background_worker.cpp
 *
 *  Created on: May 21, 2013
 *      Author: Vadim Fedorov
 */

#include "background_worker.h"

Background_Worker::Background_Worker()
{
	_work_finish_dispatcher.connect( sigc::mem_fun(*this, &Background_Worker::finish) );
	_portial_result_dispatcher.connect( sigc::mem_fun(*this, &Background_Worker::pass_data_portion) );

	_watchdog = 0;
}


void Background_Worker::start(sigc::slot1<void,Background_Worker> working_function)
{
	// bind self as a parameter
	sigc::slot entry_point = sigc::bind<Background_Worker >( working_function, *this);

	try
	{
		_thread = Glib::Threads::Thread::create( entry_point );
	}
	catch(Glib::Threads::ThreadError &err)
	{
		//TODO: notify user abour error and log it
		return;
	}

}


void Background_Worker::cancel()
{
	{
		Glib::Threads::Mutex::Lock lock(_background_work_mutex);
		_aux_stop_background_work_flag = true;
	}
}


IWatchdog* Background_Worker::get_watchdog()
{
	if (!_watchdog) {
		// Prepare watchdog for catching possible cancellation command from user
		SignalWatchdog *watchdog = new SignalWatchdog();
		watchdog->signal_ask_permission().connect( sigc::mem_fun(*this, &Background_Worker::allow_background_computation ) );
		_watchdog = watchdog;
	}

	return _watchdog;
}


void Background_Worker::submit_data_portion(I_Data* data)
{
	{
		Glib::Threads::Mutex::Lock lock(_background_work_mutex);
		_aux_partial_data = data;
	}

	// Notify main thread
	_portial_result_dispatcher();
}


void Background_Worker::announce_completion()
{
	_work_finish_dispatcher();
}


/* Private */

bool Background_Worker::allow_background_computation()
{
	bool stop_flag;

	{
		Glib::Threads::Mutex::Lock lock(_background_work_mutex);
		stop_flag = _aux_stop_background_work_flag;
	}

	return !stop_flag;
}


void Background_Worker::pass_data_portion()
{
	I_Data* partial_data;
	{
		Glib::Threads::Mutex::Lock lock(_background_work_mutex);
		partial_data = _aux_partial_data;
	}
	_signal_data_prepared.emit(partial_data);
}


void Background_Worker::finish()
{
	_thread->join();
	_signal_work_finished.emit();
}


