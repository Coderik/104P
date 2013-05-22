/*
 * BackgroundWorker.cpp
 *
 *  Created on: May 21, 2013
 *      Author: Vadim Fedorov
 */

#include "background_worker.h"

BackgroundWorker::BackgroundWorker()
{
	_work_finish_dispatcher.connect( sigc::mem_fun(*this, &BackgroundWorker::finish) );
	_portial_result_dispatcher.connect( sigc::mem_fun(*this, &BackgroundWorker::pass_data_portion) );

	_watchdog = 0;
	_thread = 0;
}


void BackgroundWorker::start(sigc::slot1<void,IBackgroundInsider* > working_function)
{
	// bind self as an IBackgroundInsider parameter
	sigc::slot<void> entry_point = sigc::bind<IBackgroundInsider* >( working_function, this );

	_aux_stop_background_work_flag = false;

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


void BackgroundWorker::cancel()
{
	{
		Glib::Threads::Mutex::Lock lock(_background_work_mutex);
		_aux_stop_background_work_flag = true;
	}
}


void BackgroundWorker::wait()
{
	if (_thread) {
		_thread->join();
	}
}


IWatchdog* BackgroundWorker::get_watchdog()
{
	if (!_watchdog) {
		// Prepare watchdog for catching possible cancellation command from user
		SignalWatchdog *watchdog = new SignalWatchdog();
		watchdog->signal_ask_permission().connect( sigc::mem_fun(*this, &BackgroundWorker::allow_background_computation ) );
		_watchdog = watchdog;
	}

	return _watchdog;
}


void BackgroundWorker::submit_data_portion(IData* data)
{
	{
		Glib::Threads::Mutex::Lock lock(_background_work_mutex);
		_aux_partial_data = data;
	}

	// Notify main thread
	_portial_result_dispatcher();
}


void BackgroundWorker::announce_completion()
{
	_work_finish_dispatcher();
}


/* Private */

bool BackgroundWorker::allow_background_computation()
{
	bool stop_flag;

	{
		Glib::Threads::Mutex::Lock lock(_background_work_mutex);
		stop_flag = _aux_stop_background_work_flag;
	}

	return !stop_flag;
}


void BackgroundWorker::pass_data_portion()
{
	IData* partial_data;
	{
		Glib::Threads::Mutex::Lock lock(_background_work_mutex);
		partial_data = _aux_partial_data;
	}
	_signal_data_prepared.emit(partial_data);
}


void BackgroundWorker::finish()
{
	_thread->join();
	_signal_work_finished.emit();
}


