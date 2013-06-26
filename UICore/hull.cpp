/*
 * hull.cpp
 *
 *  Created on: Jan 9, 2013
 *      Author: Vadim Fedorov
 */

#include "hull.h"

/* Public */

Hull::Hull(string application_id)
{
	_ui.setup_ui(this, application_id);

	_ui.quit_action->signal_activate().connect( sigc::ptr_fun(&Gtk::Main::quit) );
	_ui.open_image_action->signal_activate().connect( sigc::mem_fun(*this, &Hull::open_image) );
	_ui.open_sequence_action->signal_activate().connect( sigc::mem_fun(*this, &Hull::open_sequence) );
	_ui.open_recent_action->signal_item_activated().connect( sigc::mem_fun(*this, &Hull::open_recent) );
	_ui.calculate_optical_flow_action->signal_activate().connect( sigc::mem_fun(*this, &Hull::begin_full_optical_flow_calculation) );
	_ui.proceed_optical_flow_action->signal_activate().connect( sigc::mem_fun(*this, &Hull::begin_missing_optical_flow_calculation) );
	_ui.restore_optical_flow_action->signal_activate().connect( sigc::mem_fun(*this, &Hull::restore_optical_flow) );
	_ui.background_work_infobar->signal_response().connect( sigc::mem_fun(*this, &Hull::perceive_background_worker) );
	_ui.layers_visibility_toggle_action->signal_toggled().connect( sigc::mem_fun(*this, &Hull::set_layers_visibility) );
	_ui.signal_view_changed().connect( sigc::mem_fun(*this, &Hull::update_view) );
	_ui.signal_fitting_changed().connect( sigc::mem_fun(*this, &Hull::update_fitting) );

	_ui.image_control->signal_left_button_pressed().connect( sigc::mem_fun(*this, &Hull::left_button_pressed) );
	_ui.image_control->signal_left_button_released().connect( sigc::mem_fun(*this, &Hull::left_button_released) );
	_ui.image_control->signal_left_button_drag().connect( sigc::mem_fun(*this, &Hull::left_button_drag) );
	_ui.time_slider->signal_value_changed().connect( sigc::mem_fun(*this, &Hull::set_time) );

	this->signal_key_press_event().connect( sigc::mem_fun(*this, &Hull::key_pressed) );

	// Note: if there are several different works to compute in background, separate backgroung_worker might be used for each
	_background_worker = new BackgroundWorker();
	_background_worker->signal_data_prepared().connect( sigc::mem_fun(*this, &Hull::take_optical_flow_frame) );
	_background_worker->signal_work_finished().connect( sigc::mem_fun(*this, &Hull::end_calculate_optical_flow) );

	_sequence = NULL;
	_current_fitting = NULL;
	_has_optical_flow_data = false;

	_ui.background_work_infobar->hide();
	_ui.optical_flow_action_group->set_sensitive(false);
	_ui.view_action_group->set_sensitive(false);
	_ui.time_slider->set_sensitive(false);
	_ui.layers_visibility_toggle_action->set_active(true);

	// TODO: add preferences and controls for that api
	// NOTE: code for ImageViewer api testing
	_ui.image_control->set_handy_pan_enabled(true);
	_ui.image_control->set_zoom_by_wheel_enabled(true);
}

Hull::~Hull()
{

}


/*************************
 * I_Rig_Manager members *
 *************************/
void Hull::add_rig(Rig* rig, std::string display_name)
{
	if (rig) {
		Fitting* fitting = new Fitting();
		fitting->display_name = display_name;
		fitting->rig = rig;
		_fittings.push_back(fitting);
	}
}

void Hull::initialize_rigs()
{
	vector<Fitting* >::iterator it;
	for (it = _fittings.begin(); it != _fittings.end(); ++it) {
		(*it)->rig->initialize(this);
	}
	_ui.set_fittings(_fittings);
}


/* slots */
void Hull::open_image()
{
	Gtk::FileChooserDialog dialog("Please choose an image", Gtk::FILE_CHOOSER_ACTION_OPEN);
	dialog.set_transient_for(*this);

	//Add response buttons the the dialog:
	dialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
	dialog.add_button(Gtk::Stock::OPEN, Gtk::RESPONSE_OK);

	//Add filters, so that only certain file types can be selected:
	Glib::RefPtr<Gtk::FileFilter> filter_pgm = Gtk::FileFilter::create();
	filter_pgm->set_name("PGM image files (*.pgm)");
	filter_pgm->add_pattern("*.pgm");
	dialog.add_filter(filter_pgm);

	//Show the dialog and wait for a user response:
	int result = dialog.run();

	if (result == Gtk::RESPONSE_OK) {
		std::string filename = dialog.get_filename();
		load_image(filename);
		_ui.add_recent_file(filename);
	}
}


void Hull::load_image(string filename)
{
	Image<float> *image = ReadPgmImage(&filename);

	if (!image) {
		return;
	}

	// Adjust UI
	_ui.layer_action_group->set_sensitive(true);
	_ui.optical_flow_action_group->set_sensitive(false);
	_ui.view_action_group->set_sensitive(false);
	_ui.time_slider->set_sensitive(false);
	_ui.time_slider->set_range(0, 0);

	// Set default values
	_current_time = 0;
	_has_optical_flow_data = false;

	// Replace single image with sequence of the only element for computational uniformity.
	_sequence = new Sequence<float>(image);

	// Show image
	update_image_control(_current_time);

	// Notify rig that sequence have been changed.
	_current_fitting->rig->sequence_changed();
}


void Hull::open_sequence()
{
	Gtk::FileChooserDialog dialog("Please choose a sequence folder", Gtk::FILE_CHOOSER_ACTION_SELECT_FOLDER);
	dialog.set_transient_for(*this);

	//Add response buttons the the dialog:
	dialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
	dialog.add_button("Select", Gtk::RESPONSE_OK);

	int result = dialog.run();

	if (result == Gtk::RESPONSE_OK) {
		std::string sequence_folder = dialog.get_filename();
		load_sequence(sequence_folder);
		_ui.add_recent_folder(sequence_folder);
	}
}


void Hull::load_sequence(string path)
{
	path = path.append("/");	//TODO: check if it is needed

	// Get the list of frame names.
	Glib::RefPtr<Gio::File> dir = Gio::File::create_for_path(path);
	Glib::RefPtr<Gio::FileEnumerator> child_enumeration = dir->enumerate_children();
	Glib::RefPtr<Gio::FileInfo> file_info;
	std::vector<Glib::ustring> file_names;
	while ((file_info = child_enumeration->next_file()) != NULL)
	{
		std::string file_mime_type = file_info->get_content_type();
		// COMPATIBILITY: first for unix, second for win
		if (file_mime_type.compare("image/x-portable-graymap") == 0 || file_mime_type.compare(".pgm") == 0) {
			std::string file_name = file_info->get_name();
			file_names.push_back(file_name);
		}
	}

	if (file_names.size() == 0)
		return;

	// Sort filenames in ascending order using default < operation
	std::sort(file_names.begin(), file_names.end());

	// Load first frame, that will define width and height for the sequence.
	std::string first_frame_path = path;
	first_frame_path.append(file_names[0]);
	Image<float> *first_frame = ReadPgmImage(&first_frame_path);

	// [Re]create sequence instance
	if (_sequence)
		delete _sequence;
	_sequence = new Sequence<float>(first_frame);

	// Load the rest frames.
	for (unsigned int i = 1; i < file_names.size(); i++) {
		string frame_path = path;
		frame_path.append(file_names[i]);
		Image<float> *frame = ReadPgmImage(&frame_path);
		_sequence->add_frame(frame);
	}

	// Set default values
	_sequence_folder = path;
	_current_time = 0;
	_has_optical_flow_data = false;

	// Adjust UI
	_ui.set_view(UI_Container::VIEW_ORIGINAL_IMAGE);
	_ui.layer_action_group->set_sensitive(true);
	_ui.optical_flow_action_group->set_sensitive(true);
	_ui.view_action_group->set_sensitive(true);
	_ui.time_slider->set_sensitive(true);
	_ui.time_slider->set_range(0, _sequence->get_size_t() - 1);
	_ui.time_slider->set_digits(0);
	std::string optical_flow_file_name = path;
	optical_flow_file_name.append("optical_flow_data");

	OFStatus status = check_optical_flow(optical_flow_file_name, _sequence->get_size_x(), _sequence->get_size_y(), 2 * (_sequence->get_size_t() - 1));
	bool is_optical_flow_calculated = false;
	_optical_flow_legacy_format = false;
	if (status == STATUS_OK) {
		is_optical_flow_calculated = true;
	} else if (status == STATUS_NOT_VALID) {
		status = check_optical_flow_legacy(optical_flow_file_name, _sequence->get_size_x(), _sequence->get_size_y(), _sequence->get_size_t());
		if (status == STATUS_LEGACY_FORMAT) {
			_optical_flow_legacy_format = true;
			is_optical_flow_calculated = true;
		}
	}

	_ui.restore_optical_flow_action->set_sensitive(is_optical_flow_calculated);
	// NOTE: no auto restore, so menu item could be shaded and motion compensation denied
	_ui.allow_optical_flow_views(false);
	_ui.proceed_optical_flow_action->set_sensitive(false);
	//_ui.motion_compensation_picker->set_sensitive(false); // TODO: ?????

	// [Re]set optical flow
	reset_vector_of_pointers(_forward_optical_flow_list, _sequence->get_size_t() - 1);
	reset_vector_of_pointers(_backward_optical_flow_list, _sequence->get_size_t() - 1);

	// Show first frame
	update_image_control(_current_time);

	// Notify rig that sequence have been changed.
	_current_fitting->rig->sequence_changed();
}
	
	
void Hull::open_recent()
{
	Glib::RefPtr<Gtk::RecentInfo> current = _ui.open_recent_action->get_current_item();
	string mime_type = current->get_mime_type();
	string uri = current->get_uri();
	string path = Glib::filename_from_uri(uri);

	if (mime_type.compare("application/x-ext-pgm") == 0 || mime_type.compare("image/x-portable-graymap") == 0) {	
		load_image(path);		// WIN: "application/x-ext-pgm"; Linux: "image/x-portable-graymap"
	} else {
		load_sequence(path);	// WIN: "application/octet-stream"; Linux: "inode/directory"
	}
}


/*****************
 * IHull members *
 *****************/
Sequence<float>* Hull::request_sequence()
{
	return _sequence;
}


vector<OpticalFlowContainer*> Hull::request_forward_optical_flow()
{
	// TODO: check if absent
	return _forward_optical_flow_list;
}


vector<OpticalFlowContainer*> Hull::request_backward_optical_flow()
{
	// TODO: check if absent
	return _backward_optical_flow_list;
}


bool Hull::request_has_optical_flow_data()
{
	return _has_optical_flow_data;
}


LayerManager* Hull::request_layer_manager()
{
	if (!_current_fitting) {
		return NULL;
	}

	if (!_current_fitting->layer_manager) {
		_current_fitting->layer_manager = new LayerManager();
		_current_fitting->layer_manager->set_current_time(_current_time);
		_ui.image_control->set_layer_manager(_current_fitting->layer_manager);
	}

	return _current_fitting->layer_manager;
}


Gtk::Box* Hull::request_ui_placeholder()
{
	return _ui.right_side_layout;
}


int Hull::request_current_time()
{
	return _current_time;
}


template <typename T>
void Hull::reset_vector_of_pointers(std::vector<T*> &v, int size)
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


void Hull::set_layers_visibility()
{
	bool visibility = _ui.layers_visibility_toggle_action->get_active();
	if (visibility != _layers_visibility) {
		_layers_visibility = visibility;
		if (_current_fitting && _current_fitting->layer_manager) {
			_current_fitting->layer_manager->set_visibility(_layers_visibility);
		}
	}
}


void Hull::left_button_pressed(MouseEvent mouse_event)
{
	_current_fitting->rig->left_button_pressed(mouse_event);
}


void Hull::left_button_released(MouseEvent mouse_event)
{
	_current_fitting->rig->left_button_released(mouse_event);
}


void Hull::left_button_drag(MouseEvent mouse_event)
{
	_current_fitting->rig->left_button_drag(mouse_event);
}


void Hull::set_time()
{
	_current_time = _ui.time_slider->get_value();
	update_image_control(_current_time);

	if (_current_fitting->layer_manager) {
		_current_fitting->layer_manager->set_current_time(_current_time);
	}

	_current_fitting->rig->current_time_changed();
}


bool Hull::key_pressed(GdkEventKey* event)
{
	_current_fitting->rig->key_pressed(event);

	return false;
}


void Hull::perceive_background_worker(int responce_id)
{
	if (responce_id == 0)
		cancel_calculate_optical_flow();
}


void Hull::store_optical_flow(OpticalFlowContainer &flow, int index)
{
	if (_sequence_folder.empty())
		return;

	std::string file_name = _sequence_folder;
	file_name.append("optical_flow_data");

	int chunks_count = 2 * (_sequence->get_size_t() - 1);

	update_or_overwrite_flow(file_name, flow, index, chunks_count);
}

void Hull::restore_optical_flow()
{
	if (_sequence_folder.empty())
		return;

	std::string file_name = _sequence_folder;
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
			int chunks_count = 2 * (_sequence->get_size_t() - 1);
			int i = 0;
			for(it = _forward_optical_flow_list.begin(); it != _forward_optical_flow_list.end(); ++it, i++) {
				OpticalFlowContainer *flow = *it;
				if (flow->contains_data()) {
					update_or_overwrite_flow(file_name, *flow, i, chunks_count);
				}
			}
		}
	}

	if (has_some_data) {
		_ui.allow_optical_flow_views(true);
	}

	if (has_some_data != _has_optical_flow_data) {
		_has_optical_flow_data = has_some_data;
		_current_fitting->rig->optical_flow_changed();
	}

	fill_task_list(_forward_optical_flow_list, _backward_optical_flow_list, _task_list);
	if (_task_list.size() > 0) {
		_ui.proceed_optical_flow_action->set_sensitive(true);
	} else {
		_ui.restore_optical_flow_action->set_sensitive(false);
	}
}


/******************************************************
 * Fills the list of tasks for optical flow computation
 */
void Hull::fill_task_list(std::vector<OpticalFlowContainer*> &forward_flow, std::vector<OpticalFlowContainer*> &backward_flow, std::vector<int> &task_list)
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


void Hull::update_view()
{
	update_image_control(_current_time);
}

void Hull::update_fitting()
{
	if (_current_fitting) {
		_current_fitting->rig->deactivate();
		_ui.clear_placeholders();
		_ui.image_control->drop_layer_manager();
	}

	_current_fitting = _ui.get_fitting();
	_current_fitting->rig->activate();

	_ui.refresh_placeholders();

	if (_current_fitting->layer_manager) {
		_current_fitting->layer_manager->set_current_time(_current_time);
		_ui.image_control->set_layer_manager(_current_fitting->layer_manager);
	}
}

/* private */
int Hull::write_flow(float *u, float *v, int w, int h)
{
	FILE *fp;
	int i, j, offset;

	fp = std::fopen("flow.data", "w");

	offset = 0;

	for(i = 0; i < h; i++) {
		for(j = 0; j < w; j++, offset++) {
			std::fprintf(fp, "%d %d %f %f\n", j, h - 1 - i, u[offset], -v[offset]);
		}
		std::fprintf(fp, "\n");
	}

	std::fclose(fp);

	return 1;
}

/**
 * Converts Image into Gdk::Pixbuf
 */
Glib::RefPtr<Gdk::Pixbuf> Hull::wrap_raw_image_data(Image<float> *image)
{
	const int BITS_PER_CHANNEL = 8;

	int x_size = image->get_size_x();
	int y_size = image->get_size_y();

	Glib::RefPtr<Gdk::Pixbuf> buffer = Gdk::Pixbuf::create(Gdk::COLORSPACE_RGB, false, BITS_PER_CHANNEL, x_size, y_size);
	int rowstride = buffer->get_rowstride();	// get internal row length
	int number_of_channels = buffer->get_n_channels();
	guint8* data = buffer->get_pixels();

	int index;
	for (int y = 0;y < y_size;y++) {
		for (int x = 0;x < x_size;x++) {
			char intensity = (unsigned char)image->get_value(x, y);
			for (int c = 0; c < number_of_channels; c++) {
				/// NOTE: 'rowstride' is the length of internal representation of a row
				/// inside the Pixbuf, and it could differ from x_size*number_of_channels.
				index = y*rowstride + x*number_of_channels + c;
				data[index] = intensity;
			}
		}
	}

	return buffer;
}


void Hull::update_image_control(int current_time)
{
	UI_Container::View view = _ui.get_view();

	Glib::RefPtr<Gdk::Pixbuf> buffer;

	if (view == UI_Container::VIEW_ORIGINAL_IMAGE) {
		buffer = wrap_raw_image_data(_sequence->get_frame(current_time));
	} else if (view == UI_Container::VIEW_FORWARD_OF_COLOR) {
		if ((unsigned)current_time < _forward_optical_flow_list.size() &&
				_forward_optical_flow_list[current_time] &&
				_forward_optical_flow_list[current_time]->contains_data()) {
			buffer = static_cast<OpticalFlow*>(_forward_optical_flow_list[current_time])->get_color_code_view();
		} else {
			buffer = create_empty_pixbuf(_sequence->get_size_x(), _sequence->get_size_y());
		}
	} else if (view == UI_Container::VIEW_FORWARD_OF_GRAY) {
		if ((unsigned)current_time < _forward_optical_flow_list.size() &&
				_forward_optical_flow_list[current_time] &&
				_forward_optical_flow_list[current_time]->contains_data()) {
			buffer = static_cast<OpticalFlow*>(_forward_optical_flow_list[current_time])->get_magnitudes_view();
		} else {
			buffer = create_empty_pixbuf(_sequence->get_size_x(), _sequence->get_size_y());
		}
	} else if (view == UI_Container::VIEW_BACKWARD_OF_COLOR) {
		if (current_time > 0 &&
				(unsigned)(current_time - 1) < _backward_optical_flow_list.size() &&
				_backward_optical_flow_list[current_time - 1] &&
				_backward_optical_flow_list[current_time - 1]->contains_data()) {
			buffer = static_cast<OpticalFlow*>(_backward_optical_flow_list[current_time - 1])->get_color_code_view();
		} else {
			buffer = create_empty_pixbuf(_sequence->get_size_x(), _sequence->get_size_y());
		}
	} else if (view == UI_Container::VIEW_BACKWARD_OF_GRAY) {
		if (current_time > 0 &&
				(unsigned)(current_time - 1) < _backward_optical_flow_list.size() &&
				_backward_optical_flow_list[current_time - 1] &&
				_backward_optical_flow_list[current_time - 1]->contains_data()) {
			buffer = static_cast<OpticalFlow*>(_backward_optical_flow_list[current_time - 1])->get_magnitudes_view();
		} else {
			buffer = create_empty_pixbuf(_sequence->get_size_x(), _sequence->get_size_y());
		}
	}

	_ui.image_control->set_pixbuf(buffer);
	_ui.image_control->queue_draw();
}


Glib::RefPtr<Gdk::Pixbuf> Hull::create_empty_pixbuf(int width, int height)
{
	Glib::RefPtr<Gdk::Pixbuf> empty_image = Gdk::Pixbuf::create(Gdk::COLORSPACE_RGB, false, 8, width, height);
	empty_image->fill(0);
	return empty_image;
}

void Hull::show_status_message(std::string message)
{
	_ui.status_bar->pop(0);
	_ui.status_bar->push(message, 0);
}


void Hull::begin_full_optical_flow_calculation()
{
	// NOTE: if specify no tasks, optical flow for all frame pairs will be computed
	std::vector<int> empty_task_list;
	begin_optical_flow_calculation_internal(empty_task_list);
}


void Hull::begin_missing_optical_flow_calculation()
{
	if (_task_list.size() == 0) {
		_ui.proceed_optical_flow_action->set_sensitive(false);
		return;
	}

	begin_optical_flow_calculation_internal(_task_list);
}


void Hull::begin_optical_flow_calculation_internal(std::vector<int> task_list)
{
	_progress_counter = 0;
	_progress_total = task_list.size() != 0 ? task_list.size() : 2 * (_sequence->get_size_t() - 1);

	// Encapsulate entry point function and required parameters into slot object
	sigc::slot1<void, IBackgroundInsider*> work = sigc::bind<std::vector<int> >( sigc::mem_fun(*this, &Hull::calculate_optical_flow), task_list);

	// Run background processing
	_background_worker->start(work);

	Glib::ustring message = Glib::ustring::compose("Optical flow. Frames finished: %1; frames left: %2.", 0, _progress_total);
	_ui.background_work_infobar_message->set_text(message);
	_ui.calculate_optical_flow_action->set_sensitive(false);
	_ui.proceed_optical_flow_action->set_sensitive(false);
	_ui.background_work_infobar->show();
}


void Hull::end_calculate_optical_flow()
{
	_ui.background_work_infobar->hide();
	_ui.calculate_optical_flow_action->set_sensitive(true);

	fill_task_list(_forward_optical_flow_list, _backward_optical_flow_list, _task_list);
	if (_task_list.size() > 0) {
		_ui.proceed_optical_flow_action->set_sensitive(true);
	} else {
		_ui.restore_optical_flow_action->set_sensitive(false);
	}
}


void Hull::take_optical_flow_frame(IData *data)
{
	// Cast calculated values
	OpticalFlowData *optical_flow_data = dynamic_cast<OpticalFlowData*>(data);
	if (!optical_flow_data) {
		return;
	}

	int size_x = _sequence->get_size_x();
	int size_y = _sequence->get_size_y();
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
	flow->set_flow(optical_flow_data->flow_x, optical_flow_data->flow_y, size_x, size_y);

	// Update progress
	_progress_counter++;
	Glib::ustring message = Glib::ustring::compose("Optical flow. Frames finished: %1; frames left: %2.", _progress_counter, _progress_total - _progress_counter);
	_ui.background_work_infobar_message->set_text(message);

	_ui.allow_optical_flow_views(true);
	_has_optical_flow_data = true;
	_current_fitting->rig->optical_flow_changed();

	store_optical_flow(*flow, index);

	_ui.restore_optical_flow_action->set_sensitive(true);
}


void Hull::cancel_calculate_optical_flow()
{
	_ui.background_work_infobar_message->set_text("Optical flow. Canceling... ");
	_background_worker->cancel();
}


/******************************************************************************************************
 * If pass no tasks, forward and backward optical flow fields for all pairs of frames will be computed.
 * In task list absolute value defines from what frame to compute flow, and sign defines direction.
 * Example: '0' - forward flow from frame 0 to frame 1; '-2' - backward flow from frame 2 to frame 1.
 */
void Hull::calculate_optical_flow(IBackgroundInsider *insider, std::vector<int> task_list)
{
	if (!_sequence)
		return;

	// Get watchdog for catching possible cancellation command from user
	IWatchdog *watchdog = insider->get_watchdog();

	// Create with default parameters
	Zach_TVL1_OpticalFlow* opticalFlow = new Zach_TVL1_OpticalFlow(false);
	opticalFlow->set_watchdog(watchdog);

	int x_size = _sequence->get_size_x();
	int y_size = _sequence->get_size_y();
	int frame_count = _sequence->get_size_t();

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
		float* first_frame_data = _sequence->get_frame(first_index)->get_raw_data();
		float* second_frame_data = _sequence->get_frame(second_index)->get_raw_data();

		// Allocate memory for the flow
		float *u1 = new float[x_size * y_size];
		float *u2 = new float[x_size * y_size];

		// Calculate optical flow
		//me_prefilter(first_frame_data, second_frame_data, x_size, y_size);
		bool success = opticalFlow->calculate_with_multiscale(first_frame_data, second_frame_data, u1, u2, x_size, y_size);

		if (success) {
			// Set calculated values
			OpticalFlowData *data = new OpticalFlowData();
			data->flow_x = u1;
			data->flow_y = u2;
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



