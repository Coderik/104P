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

#include "background_worker.h"

BackgroundWorker::BackgroundWorker()
{
	_work_finish_dispatcher.connect( sigc::mem_fun(*this, &BackgroundWorker::finish) );
	_portial_result_dispatcher.connect( sigc::mem_fun(*this, &BackgroundWorker::pass_data_portion) );

	_watchdog = 0;
	_thread = 0;
}


BackgroundWorker::~BackgroundWorker()
{
	if (_watchdog) {
		delete _watchdog;
	}
}


/**
 * Starts background processing.
 * NOTE: This method is called from the UI thread.
 */
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


/**
 * Cancels background processing.
 * NOTE: This method is called from the UI thread.
 */
void BackgroundWorker::cancel()
{
	{
		Glib::Threads::Mutex::Lock lock(_background_work_mutex);
		_aux_stop_background_work_flag = true;
	}
}


/**
 * Waits until background thread is finished.
 * NOTE: This method is called from the UI thread.
 */
void BackgroundWorker::wait()
{
	if (_thread) {
		_thread->join();
	}
}


/**
 * Returns a watchdog.
 * NOTE: This method is called from the background thread.
 */
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


/**
 * Transfers portion of data to UI thread side.
 * NOTE: This method is called from the background thread.
 */
void BackgroundWorker::submit_data_portion(IData* data)
{
	{
		Glib::Threads::Mutex::Lock lock(_background_work_mutex);
		_aux_partial_data = data;
	}

	// Notify UI thread
	_portial_result_dispatcher();
}


/**
 * Notifies UI thread side about work completion.
 * NOTE: This method is called from the background thread.
 */
void BackgroundWorker::announce_completion()
{
	_work_finish_dispatcher();
}


/* Private */

/**
 * Checks the flag to make sure that work might be continued.
 * NOTE: This method is called from the background thread.
 */
bool BackgroundWorker::allow_background_computation()
{
	bool stop_flag;

	{
		Glib::Threads::Mutex::Lock lock(_background_work_mutex);
		stop_flag = _aux_stop_background_work_flag;
	}

	return !stop_flag;
}


/**
 * Transfers portion of data to the client.
 * NOTE: This method is called via dispatcher and therefore runs in the UI thread.
 */
void BackgroundWorker::pass_data_portion()
{
	IData* partial_data;
	{
		Glib::Threads::Mutex::Lock lock(_background_work_mutex);
		partial_data = _aux_partial_data;
	}
	_signal_data_prepared.emit(partial_data);
}


/**
 * Joins the background thread and notifies the client about work completion.
 * NOTE: This method is called via dispatcher and therefore runs in the UI thread.
 */
void BackgroundWorker::finish()
{
	_thread->join();
	_signal_work_finished.emit();
}


