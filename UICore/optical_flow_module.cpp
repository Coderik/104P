/*
 * optical_flow_module.cpp
 *
 *  Created on: Feb 17, 2014
 *      Author: Vadim Fedorov
 */

#include "optical_flow_module.h"

void OpticalFlowModule::initialize(IModulable *modulable)
{
	_modulable = modulable;
	_has_optical_flow_data = false;

	_modulable->signal_sequence_changed().connect( sigc::mem_fun(*this, &OpticalFlowModule::sequence_changed) );

	// create menu item for the module and assign it to the main menu
	_menu = Gtk::manage(new Gtk::Menu());
	_restore_menu_item = Gtk::manage(new Gtk::MenuItem("Restore Optical Flow"));
	_restore_menu_item->signal_activate().connect( sigc::mem_fun(*this, &OpticalFlowModule::restore_optical_flow) );
	_menu->append(*_restore_menu_item);
	Gtk::MenuItem *separator = Gtk::manage(new Gtk::SeparatorMenuItem());
	_menu->append(*separator);
	_calculate_menu_item = Gtk::manage(new Gtk::MenuItem("Calculate Optical Flow"));
	_calculate_menu_item->signal_activate().connect( sigc::mem_fun(*this, &OpticalFlowModule::begin_full_optical_flow_calculation) );
	_menu->append(*_calculate_menu_item);
	_proceed_menu_item = Gtk::manage(new Gtk::MenuItem("Proceed Calculaton"));
	_proceed_menu_item->signal_activate().connect( sigc::mem_fun(*this, &OpticalFlowModule::begin_missing_optical_flow_calculation) );
	_menu->append(*_proceed_menu_item);
	_menu->set_sensitive(false);
	_modulable->assign_menu(_menu, "Optical Flow");

	// Note: if there are several different works to compute in background, separate backgroung_worker might be used for each
	_background_worker = new BackgroundWorker();
	_background_worker->signal_data_prepared().connect( sigc::mem_fun(*this, &OpticalFlowModule::take_optical_flow_frame) );
	_background_worker->signal_work_finished().connect( sigc::mem_fun(*this, &OpticalFlowModule::end_calculate_optical_flow) );
}


vector<OpticalFlowContainer*> OpticalFlowModule::request_forward_optical_flow()
{
	// TODO: check if absent
	return _forward_optical_flow_list;
}


vector<OpticalFlowContainer*> OpticalFlowModule::request_backward_optical_flow()
{
	// TODO: check if absent
	return _backward_optical_flow_list;
}


bool OpticalFlowModule::request_has_optical_flow_data()
{
	return _has_optical_flow_data;
}

/* Private */

void OpticalFlowModule::sequence_changed()
{
	remove_optical_flow_views();
	_has_optical_flow_data = false;

	Sequence<float> *sequence = _modulable->request_sequence();

	if (!sequence) {
		return;
	}

	_frames_amount = sequence->get_size_t();

	if (_frames_amount > 1) {
		_menu->set_sensitive(true);

		// ?TODO: cancel background work if any

		std::string optical_flow_file_name = _modulable->request_sequence_path();
		optical_flow_file_name.append("optical_flow_data");

		OFStatus status = check_optical_flow(optical_flow_file_name, sequence->get_size_x(), sequence->get_size_y(), 2 * (sequence->get_size_t() - 1));
		bool is_optical_flow_calculated = false;
		_optical_flow_legacy_format = false;
		if (status == STATUS_OK) {
			is_optical_flow_calculated = true;
		} else if (status == STATUS_NOT_VALID) {
			status = check_optical_flow_legacy(optical_flow_file_name, sequence->get_size_x(), sequence->get_size_y(), sequence->get_size_t());
			if (status == STATUS_LEGACY_FORMAT) {
				_optical_flow_legacy_format = true;
				is_optical_flow_calculated = true;
			}
		}

		_restore_menu_item->set_sensitive(is_optical_flow_calculated);
		// NOTE: no auto restore, so menu item could be shaded
		_proceed_menu_item->set_sensitive(false);

		// [Re]set optical flow
		reset_vector_of_pointers(_forward_optical_flow_list, sequence->get_size_t() - 1);
		reset_vector_of_pointers(_backward_optical_flow_list, sequence->get_size_t() - 1);
	} else {
		_menu->set_sensitive(false);
	}
}


/**
 * Slot for 'Restore Optical Flow' menu item activation signal
 */
void OpticalFlowModule::restore_optical_flow()
{
	string sequence_folder = _modulable->request_sequence_path();

	if (sequence_folder.empty()) {
		return;
	}

	std::string file_name = sequence_folder;
	file_name.append("optical_flow_data");

	std::vector<OpticalFlowContainer*>::iterator it;
	for(it = _forward_optical_flow_list.begin(); it != _forward_optical_flow_list.end(); ++it) {
		if (!*it) {
			*it = new OpticalFlow();
		}
	}
	for(it = _backward_optical_flow_list.begin(); it != _backward_optical_flow_list.end(); ++it) {
		if (!*it) {
			*it = new OpticalFlow();
		}
	}

	bool has_some_data = false;
	if (!_optical_flow_legacy_format) {
		has_some_data |= read_whole_direction_data(file_name, true, _forward_optical_flow_list);
		has_some_data |= read_whole_direction_data(file_name, false, _backward_optical_flow_list);
	} else {
		if (read_forward_optical_flow_legacy(file_name, _forward_optical_flow_list)) {
			has_some_data = true;

			// Save data in up to date format
			int chunks_count = 2 * (_frames_amount - 1);
			int i = 0;
			for(it = _forward_optical_flow_list.begin(); it != _forward_optical_flow_list.end(); ++it, i++) {
				OpticalFlowContainer *flow = *it;
				if (flow->contains_data()) {
					update_or_overwrite_flow(file_name, *flow, i, chunks_count);
				}
			}
		}
	}

	if (has_some_data != _has_optical_flow_data) {
		if (_has_optical_flow_data) {
			remove_optical_flow_views();
		} else {
			add_optical_flow_views();
		}
		_has_optical_flow_data = has_some_data;
		notify_changes();
	}

	fill_task_list(_forward_optical_flow_list, _backward_optical_flow_list, _task_list);
	if (_task_list.size() > 0) {
		_proceed_menu_item->set_sensitive(true);
	} else {
		_restore_menu_item->set_sensitive(false);
	}
}


/**
 * Slot for 'Calculate Optical Flow' menu item activation signal
 */
void OpticalFlowModule::begin_full_optical_flow_calculation()
{
	// NOTE: if specify no tasks, optical flow for all frame pairs will be computed
	std::vector<int> empty_task_list;
	begin_optical_flow_calculation_internal(empty_task_list);
}


/**
 * Slot for 'Proceed Calculaton' menu item activation signal
 */
void OpticalFlowModule::begin_missing_optical_flow_calculation()
{
	if (_task_list.size() == 0) {
		_proceed_menu_item->set_sensitive(false);
		return;
	}

	begin_optical_flow_calculation_internal(_task_list);
}


Glib::RefPtr<Gdk::Pixbuf> OpticalFlowModule::provide_forward_optical_flow_vector_view(unsigned int time)
{
	Glib::RefPtr<Gdk::Pixbuf> buffer;

	if (time < _forward_optical_flow_list.size() &&
			_forward_optical_flow_list[time] &&
			_forward_optical_flow_list[time]->contains_data()) {
		buffer = static_cast<OpticalFlow*>(_forward_optical_flow_list[time])->get_color_code_view();
	}

	return buffer;
}


Glib::RefPtr<Gdk::Pixbuf> OpticalFlowModule::provide_forward_optical_flow_magnitude_view(unsigned int time)
{
	Glib::RefPtr<Gdk::Pixbuf> buffer;

	if (time < _forward_optical_flow_list.size() &&
			_forward_optical_flow_list[time] &&
			_forward_optical_flow_list[time]->contains_data()) {
		buffer = static_cast<OpticalFlow*>(_forward_optical_flow_list[time])->get_magnitudes_view();
	}

	return buffer;
}


Glib::RefPtr<Gdk::Pixbuf> OpticalFlowModule::provide_backward_optical_flow_vector_view(unsigned int time)
{
	Glib::RefPtr<Gdk::Pixbuf> buffer;

	if (time > 0 &&
			(time - 1) < _backward_optical_flow_list.size() &&
			_backward_optical_flow_list[time - 1] &&
			_backward_optical_flow_list[time - 1]->contains_data()) {
		buffer = static_cast<OpticalFlow*>(_backward_optical_flow_list[time - 1])->get_color_code_view();
	}

	return buffer;
}


Glib::RefPtr<Gdk::Pixbuf> OpticalFlowModule::provide_backward_optical_flow_magnitude_view(unsigned int time)
{
	Glib::RefPtr<Gdk::Pixbuf> buffer;

	if (time > 0 &&
			(time - 1) < _backward_optical_flow_list.size() &&
			_backward_optical_flow_list[time - 1] &&
			_backward_optical_flow_list[time - 1]->contains_data()) {
		buffer = static_cast<OpticalFlow*>(_backward_optical_flow_list[time - 1])->get_magnitudes_view();
	}

	return buffer;
}


template <typename T>
void OpticalFlowModule::reset_vector_of_pointers(std::vector<T*> &v, int size)
{
	typename std::vector<T*>::iterator it;
	for (it = v.begin(); it != v.end(); ++it) {
		if (*it) {
			delete *it;
		}
	}
	v.clear();
	v = std::vector<T*>(size);
	for (it = v.begin(); it != v.end(); ++it) {
		*it = 0;
	}
}


void OpticalFlowModule::begin_optical_flow_calculation_internal(std::vector<int> task_list)
{
	_progress_counter = 0;
	_progress_total = task_list.size() != 0 ? task_list.size() : 2 * (_frames_amount - 1);

	// Encapsulate entry point function and required parameters into slot object
	sigc::slot1<void, IBackgroundInsider*> work = sigc::bind<std::vector<int> >( sigc::mem_fun(*this, &OpticalFlowModule::calculate_optical_flow), task_list);

	// Run background processing
	_background_worker->start(work);

	Glib::ustring message = Glib::ustring::compose("Optical flow. Frames finished: %1; frames left: %2.", 0, _progress_total);
	_background_work_info = _modulable->add_background_work_info(sigc::mem_fun(*this, &OpticalFlowModule::cancel_calculate_optical_flow), message);
	_calculate_menu_item->set_sensitive(false);
	_proceed_menu_item->set_sensitive(false);
}


/******************************************************************************************************
 * If pass no tasks, forward and backward optical flow fields for all pairs of frames will be computed.
 * In task list absolute value defines from what frame to compute flow, and sign defines direction.
 * Example: '0' - forward flow from frame 0 to frame 1; '-2' - backward flow from frame 2 to frame 1.
 */
void OpticalFlowModule::calculate_optical_flow(IBackgroundInsider *insider, std::vector<int> task_list)
{
	Sequence<float> *sequence = _modulable->request_sequence();	// ?TODO: is this thread-safe?

	if (!sequence)
		return;

	// Get watchdog for catching possible cancellation command from user
	IWatchdog *watchdog = insider->get_watchdog();

	// Create with default parameters
	Zach_TVL1_OpticalFlow* opticalFlow = new Zach_TVL1_OpticalFlow(false);
	opticalFlow->set_watchdog(watchdog);

	int size_x = sequence->get_size_x();
	int size_y = sequence->get_size_y();
	int frame_count = sequence->get_size_t();

	// Fill task list, if empty
	if (task_list.size() == 0) {
		int flow_count = frame_count - 1;	// in one direction
		task_list.reserve(flow_count * 2);

		for (int i = 0; i < flow_count; i++)
			task_list.push_back(i);
		for (int i = -1; i >= -flow_count; i--)
			task_list.push_back(i);
	}

	std::vector<int>::iterator it;
	for (it = task_list.begin(); it != task_list.end(); ++it) {
		// Get frames data
		int first_index = std::abs(*it);
		int second_index = (*it >= 0)? first_index + 1 : first_index - 1;
		float* first_frame_data = sequence->get_frame(first_index)->get_raw_data();
		float* second_frame_data = sequence->get_frame(second_index)->get_raw_data();

		// Allocate memory for the flow
		float *u1 = new float[size_x * size_y];
		float *u2 = new float[size_x * size_y];

		// Calculate optical flow
		//me_prefilter(first_frame_data, second_frame_data, x_size, y_size);
		bool success = opticalFlow->calculate_with_multiscale(first_frame_data, second_frame_data, u1, u2, size_x, size_y);

		if (success) {
			// Set calculated values
			OpticalFlowData *data = new OpticalFlowData();
			data->flow_x = u1;
			data->flow_y = u2;
			data->size_x = size_x;
			data->size_y = size_y;
			data->index = *it;

			// Notify main thread, that i-th optical flow slice is calculated
			insider->submit_data_portion(data);
		}

		// Check stop flag
		if (!watchdog->can_continue()) {
			break;
		}
	}

	// Notify main thread, that the background work is done
	insider->announce_completion();
}


void OpticalFlowModule::take_optical_flow_frame(IData *data)
{
	// Cast calculated values
	OpticalFlowData *optical_flow_data = dynamic_cast<OpticalFlowData*>(data);
	if (!optical_flow_data) {
		return;
	}

	int index = optical_flow_data->index;

	// Choose appropriate place to put flow
	OpticalFlowContainer *flow;
	if (index >= 0) {
		if (!_forward_optical_flow_list[index]) {
			_forward_optical_flow_list[index] = new OpticalFlow();
		}
		flow = _forward_optical_flow_list[index];
	} else {
		if (!_backward_optical_flow_list[-index - 1]) {
			_backward_optical_flow_list[-index - 1] = new OpticalFlow();
		}
		flow = _backward_optical_flow_list[-index - 1];
	}
	if (flow->contains_data()) {
		flow->clear();
	}
	flow->set_flow(optical_flow_data->flow_x, optical_flow_data->flow_y, optical_flow_data->size_x, optical_flow_data->size_y);

	// Update progress
	_progress_counter++;
	Glib::ustring message = Glib::ustring::compose("Optical flow. Frames finished: %1; frames left: %2.", _progress_counter, _progress_total - _progress_counter);
	_modulable->alter_background_work_info(_background_work_info, message);

	if (!_has_optical_flow_data) {
		add_optical_flow_views();
	}

	_has_optical_flow_data = true;
	notify_changes();

	store_optical_flow(*flow, index);

	_restore_menu_item->set_sensitive(true);
}


void OpticalFlowModule::end_calculate_optical_flow()
{
	_modulable->remove_background_work_info(_background_work_info);
	_calculate_menu_item->set_sensitive(true);

	fill_task_list(_forward_optical_flow_list, _backward_optical_flow_list, _task_list);
	if (_task_list.size() > 0) {
		_proceed_menu_item->set_sensitive(true);
	} else {
		_restore_menu_item->set_sensitive(false);
	}
}


void OpticalFlowModule::store_optical_flow(OpticalFlowContainer &flow, int index)
{
	string sequence_folder = _modulable->request_sequence_path();

	if (sequence_folder.empty()) {
		return;
	}

	std::string file_name = sequence_folder;
	file_name.append("optical_flow_data");

	int chunks_count = 2 * (_frames_amount - 1);

	update_or_overwrite_flow(file_name, flow, index, chunks_count);
}


void OpticalFlowModule::cancel_calculate_optical_flow()
{
	_modulable->alter_background_work_info(_background_work_info, "Optical flow. Canceling... ");
	_background_worker->cancel();
}


/******************************************************
 * Fills the list of tasks for optical flow computation
 */
void OpticalFlowModule::fill_task_list(std::vector<OpticalFlowContainer*> &forward_flow, std::vector<OpticalFlowContainer*> &backward_flow, std::vector<int> &task_list)
{
	task_list.clear();
	task_list.reserve(2 * forward_flow.size());
	int i = 0;
	std::vector<OpticalFlowContainer*>::iterator it;
	for(it = forward_flow.begin(); it != forward_flow.end(); ++it, i++) {
		if (!*it || !(*it)->contains_data()) {
			task_list.push_back(i);
		}
	}
	i = -1;
	for(it = backward_flow.begin(); it != backward_flow.end(); ++it, i--) {
		if (!*it || !(*it)->contains_data()) {
			task_list.push_back(i);
		}
	}
}


void OpticalFlowModule::notify_changes()
{
	Request<IRig, IOpticalFlowAware> request;
	_modulable->request_active_rig(request);
	IOpticalFlowAware *optical_flow_aware_rig = request.get_responce();
	if (optical_flow_aware_rig) {
		optical_flow_aware_rig->optical_flow_changed();
	}
}


void OpticalFlowModule::add_optical_flow_views()
{
	// TODO:check if already there
	_forward_optical_flow_vector_view =
			_modulable->add_view("Forward Optical Flow (direction and magnitude)", sigc::mem_fun(*this, &OpticalFlowModule::provide_forward_optical_flow_vector_view));
	_forward_optical_flow_magnitude_view =
			_modulable->add_view("Forward Optical Flow (magnitude)", sigc::mem_fun(*this, &OpticalFlowModule::provide_forward_optical_flow_magnitude_view));
	_backward_optical_flow_vector_view =
			_modulable->add_view("Backward Optical Flow (direction and magnitude)", sigc::mem_fun(*this, &OpticalFlowModule::provide_backward_optical_flow_vector_view));
	_backward_optical_flow_magnitude_view =
			_modulable->add_view("Backward Optical Flow (magnitude)", sigc::mem_fun(*this, &OpticalFlowModule::provide_backward_optical_flow_vector_view));
}


void OpticalFlowModule::remove_optical_flow_views()
{
	_modulable->remove_view(_forward_optical_flow_vector_view);
	_modulable->remove_view(_forward_optical_flow_magnitude_view);
	_modulable->remove_view(_backward_optical_flow_vector_view);
	_modulable->remove_view(_backward_optical_flow_magnitude_view);
}
