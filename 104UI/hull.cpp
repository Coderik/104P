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
	_ui.layers_visibility_toggle_action->signal_toggled().connect( sigc::mem_fun(*this, &Hull::set_layers_visibility) );
	_ui.signal_active_view_changed().connect( sigc::mem_fun(*this, &Hull::active_view_changed) );
	_ui.signal_fitting_changed().connect( sigc::mem_fun(*this, &Hull::update_fitting) );
	_ui.background_work_infobar->signal_response().connect( sigc::mem_fun(*this, &Hull::background_worker_infobar_responded) );

	_ui.image_control->signal_left_button_pressed().connect( sigc::mem_fun(*this, &Hull::left_button_pressed) );
	_ui.image_control->signal_left_button_released().connect( sigc::mem_fun(*this, &Hull::left_button_released) );
	_ui.image_control->signal_left_button_drag().connect( sigc::mem_fun(*this, &Hull::left_button_drag) );
	_ui.time_slider->signal_value_changed().connect( sigc::mem_fun(*this, &Hull::set_time) );

	this->signal_key_press_event().connect( sigc::mem_fun(*this, &Hull::key_pressed) );

	_current_fitting = NULL;
	_sequence_folder = "";

	_ui.background_work_infobar->hide();
	_ui.time_slider->set_sensitive(false);
	_ui.layers_visibility_toggle_action->set_active(true);

	// create Original Image view
	_original_image_view = add_view("Original Image", sigc::mem_fun(*this, &Hull::provide_original_image_view ));
	active_view_changed(_original_image_view);

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
		HullProxy *proxy = new HullProxy(this);
		(*it)->rig->initialize(proxy);
		(*it)->proxy = proxy;
	}
	_ui.set_fittings(_fittings);
}


/* slots */
void Hull::open_image()
{
	Gtk::FileChooserDialog dialog("Please choose an image", Gtk::FILE_CHOOSER_ACTION_OPEN);
	dialog.set_transient_for(*this);
	dialog.set_current_folder(Glib::get_current_dir());

	//Add response buttons the the dialog:
	dialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
	dialog.add_button(Gtk::Stock::OPEN, Gtk::RESPONSE_OK);

	//Add filters, so that only certain file types can be selected:
	Glib::RefPtr<Gtk::FileFilter> filter_pgm = Gtk::FileFilter::create();
	filter_pgm->set_name("PGM image files (*.pgm)");
	filter_pgm->add_pattern("*.pgm");
	dialog.add_filter(filter_pgm);
    Glib::RefPtr<Gtk::FileFilter> filter_png = Gtk::FileFilter::create();
    filter_png->set_name("PNG image files (*.png)");
    filter_png->add_pattern("*.png");
    dialog.add_filter(filter_png);

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
	Image<float> image = IOUtility::read_rgb_image(filename);   // TODO: deduce the number of channels from format

	if (!image) {
		return;
	}

	// Adjust UI
	_ui.layer_action_group->set_sensitive(true);
	_ui.time_slider->set_sensitive(false);
	_ui.time_slider->set_range(0, 0);
	active_view_changed(_original_image_view);

	// Set default values
	_sequence_folder = "";
	_current_time = 0;

	// Replace single image with sequence of the only element for computational uniformity.
	_sequence = SequenceFx<float>(image);

	// Show image
	update_image_control(_current_time);

	if (!_current_fitting) {
		return;
	}

	// Notify rig that sequence have been changed.
	_current_fitting->rig->sequence_changed();

	// notify all modules that sequence have been changed
	_signal_sequence_changed.emit();

	// Set current time for all already existing layers
	if (_current_fitting->layer_manager) {
		_current_fitting->layer_manager->set_current_time(_current_time);
	}
}


void Hull::open_sequence()
{
	Gtk::FileChooserDialog dialog("Please choose a sequence folder", Gtk::FILE_CHOOSER_ACTION_SELECT_FOLDER);
	dialog.set_transient_for(*this);
	dialog.set_current_folder(Glib::get_current_dir());

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
	while ((file_info = child_enumeration->next_file()) != 0)
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
	Image<float> first_frame = IOUtility::read_rgb_image(first_frame_path);

	// [Re]create sequence instance
	_sequence = SequenceFx<float>(first_frame);

	// Load the rest frames.
	for (unsigned int i = 1; i < file_names.size(); i++) {
		string frame_path = path;
		frame_path.append(file_names[i]);
		Image<float> frame = IOUtility::read_rgb_image(frame_path);
		_sequence.add(frame);
	}

	// Set default values
	_sequence_folder = path;
	_current_time = 0;

	// Adjust UI
	_ui.layer_action_group->set_sensitive(true);
	_ui.time_slider->set_sensitive(true);
	_ui.time_slider->set_range(0, _sequence.size_t() - 1);
	_ui.time_slider->set_digits(0);
	active_view_changed(_original_image_view);

	// Show first frame
	update_image_control(_current_time);

	if (!_current_fitting) {
		return;
	}

	// Notify rig that sequence have been changed.
	_current_fitting->rig->sequence_changed();

	// notify all modules that sequence have been changed
	_signal_sequence_changed.emit();

	// Set current time for all already existing layers
	if (_current_fitting->layer_manager) {
		_current_fitting->layer_manager->set_current_time(_current_time);
	}
}
	
	
void Hull::open_recent()
{
	Glib::RefPtr<Gtk::RecentInfo> current = _ui.open_recent_action->get_current_item();
	string mime_type = current->get_mime_type();
	string uri = current->get_uri();
	string path = Glib::filename_from_uri(uri);

	if (mime_type.compare("application/x-ext-pgm") == 0 ||
        mime_type.compare("image/x-portable-graymap") == 0 ||
        mime_type.compare("image/png") == 0) {
		load_image(path);		// WIN: "application/x-ext-pgm"; Linux: "image/x-portable-graymap"
	} else {
		load_sequence(path);	// WIN: "application/octet-stream"; Linux: "inode/directory"
	}
}


/*****************
 * IHull members *
 *****************/
SequenceFx<float> Hull::request_sequence()
{
	return _sequence;
}


LayerManager* Hull::request_layer_manager()
{
	if (!_current_fitting) {
		return 0;
	}

	if (!_current_fitting->layer_manager) {
		_current_fitting->layer_manager = new LayerManager();
		_current_fitting->layer_manager->set_current_time(_current_time);
		_ui.image_control->set_layer_manager(_current_fitting->layer_manager);
	}

	return _current_fitting->layer_manager;
}


InteractionManager* Hull::request_interaction_manager()
{
	if (!_current_fitting) {
		return 0;
	}

	if (!_current_fitting->interaction_manager) {
		_current_fitting->interaction_manager = new InteractionManager();
		_connection_interaction_manager_signal_ui_updated =
					_current_fitting->interaction_manager->signal_ui_updated().connect(
							sigc::mem_fun(*this, &Hull::update_toolbar) );
	}

	return _current_fitting->interaction_manager;
}


Gtk::Box* Hull::request_ui_placeholder()
{
	return _ui.right_side_layout;
}


int Hull::request_current_time()
{
	return _current_time;
}


void Hull::request_module(RequestBase<IModule> &request)
{
	request.match(_modules.begin(), _modules.end());
}


/**************************
 * IModuleManager members *
 **************************/

void Hull::add_module(IModule *module)
{
	// TODO: implement
	_modules.push_back(module);
	module->initialize(this);
}


sigc::signal<void> Hull::signal_sequence_changed()
{
	return _signal_sequence_changed;
}


void Hull::assign_menu(Gtk::Menu *menu, string title)
{
	_ui.assign_menu(menu, title);
}


string Hull::request_sequence_path()
{
	return _sequence_folder;
}


string Hull::request_open_dialog_result(string dialog_title, Glib::RefPtr<Gtk::FileFilter> filter)
{
	Gtk::FileChooserDialog dialog(dialog_title, Gtk::FILE_CHOOSER_ACTION_OPEN);
	dialog.set_transient_for(*this);
	dialog.set_current_folder(Glib::get_current_dir());

	// add response buttons the the dialog:
	dialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
	dialog.add_button(Gtk::Stock::OPEN, Gtk::RESPONSE_OK);

	// add filters, so that only certain file types can be selected:
	if (filter) {
		dialog.add_filter(filter);
	}

	// show the dialog and wait for a user response:
	int result = dialog.run();

	string filename;
	if (result == Gtk::RESPONSE_OK) {
		filename = dialog.get_filename();
	}

	return filename;
}


string Hull::request_save_dialog_result(string dialog_title, Glib::RefPtr<Gtk::FileFilter> filter)
{
	Gtk::FileChooserDialog dialog(dialog_title, Gtk::FILE_CHOOSER_ACTION_SAVE);
	dialog.set_transient_for(*this);
	dialog.set_current_folder(Glib::get_current_dir());

	// add response buttons the the dialog:
	dialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
	dialog.add_button(Gtk::Stock::SAVE_AS, Gtk::RESPONSE_OK);

	// add filters, so that only certain file types can be selected:
	if (filter) {
		dialog.add_filter(filter);
	}

	// show the dialog and wait for a user response:
	int result = dialog.run();

	string filename;
	if (result == Gtk::RESPONSE_OK) {
		filename = dialog.get_filename();
	}

	return filename;
}


void Hull::request_active_rig(RequestBase<IRig> &request)
{
	if (_current_fitting && _current_fitting->rig) {
		request.match(_current_fitting->rig);
	}
}


Descriptor Hull::add_view(string title, sigc::slot1<Glib::RefPtr<Gdk::Pixbuf>, int> provider)
{
	Descriptor descriptor = Descriptor::create();
	_view_map[descriptor] = new View(title, provider);

	refresh_view_menu();

	return descriptor;
}


bool Hull::alter_view(Descriptor view_descriptor, string title, sigc::slot1<Glib::RefPtr<Gdk::Pixbuf>, int> provider)
{
	if (_view_map.find(view_descriptor) == _view_map.end()) {
		return false;
	}

	View* view = _view_map[view_descriptor];
	view->set_title(title);
	view->set_provider(provider);

	refresh_view_menu();

	return true;
}


bool Hull::remove_view(Descriptor view_descriptor)
{
	if (_view_map.find(view_descriptor) == _view_map.end()) {
		return false;
	}

	_view_map.erase(view_descriptor);
	// TODO: if this was active, set another active somehow

	refresh_view_menu();

	return true;
}


bool Hull::queue_view_draw(Descriptor view_descriptor)
{
	if (_active_view == view_descriptor) {
		update_image_control(_current_time);
		return true;
	}

	return false;
}


Descriptor Hull::add_background_work_info(sigc::slot0<void> cancel_slot, string message)
{
	_background_work_info = Descriptor::create();

	_background_work_cancel_slot = cancel_slot;
	_ui.background_work_infobar_message->set_text(message);
	_ui.background_work_infobar->show();

	return _background_work_info;
}


bool Hull::alter_background_work_info(Descriptor descriptor, string message)
{
	if (descriptor == _background_work_info) {
		_ui.background_work_infobar_message->set_text(message);
		return true;
	}

	return false;
}


bool Hull::remove_background_work_info(Descriptor descriptor)
{
	if (descriptor == _background_work_info) {
		_ui.background_work_infobar->hide();
		descriptor = Descriptor::create();	//TODO: set to empty
		return true;
	}

	return false;
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


void Hull::background_worker_infobar_responded(int responce_id)
{
	if (responce_id == 0 && !_background_work_cancel_slot.empty()) {
		_background_work_cancel_slot();
	}
}


void Hull::left_button_pressed(MouseEvent mouse_event)
{
	mouse_event.t = _current_time;

	if (_current_fitting->interaction_manager) {
		Interaction *interaction = _current_fitting->interaction_manager->get_active();
		if (interaction) {
			interaction->left_button_pressed(mouse_event);
		}
	}
}


void Hull::left_button_released(MouseEvent mouse_event)
{
	mouse_event.t = _current_time;

	if (_current_fitting->interaction_manager) {
		Interaction *interaction = _current_fitting->interaction_manager->get_active();
		if (interaction) {
			interaction->left_button_released(mouse_event);
		}
	}
}


void Hull::left_button_drag(MouseEvent mouse_event)
{
	mouse_event.t = _current_time;

	if (_current_fitting->interaction_manager) {
		Interaction *interaction = _current_fitting->interaction_manager->get_active();
		if (interaction) {
			interaction->left_button_drag(mouse_event);
		}
	}
}


void Hull::set_time()
{
	_current_time = _ui.time_slider->get_value();
	update_image_control(_current_time);

	if (_current_fitting) {
		if (_current_fitting->layer_manager) {
			_current_fitting->layer_manager->set_current_time(_current_time);
		}

		_current_fitting->rig->current_time_changed();
	}
}


bool Hull::key_pressed(GdkEventKey* event)
{
	if (!_current_fitting) {
		return false;
	}

	if (_current_fitting->interaction_manager) {
		Interaction *interaction = _current_fitting->interaction_manager->get_active();
		if (interaction) {
			interaction->key_pressed(event);
		}
	}

	// TODO: It might be useful to add some is_handled flag to the event,
	//       because it is normal to have more than one receiver.
	_current_fitting->rig->key_pressed(event);

	return false;
}


Glib::RefPtr<Gdk::Pixbuf> Hull::provide_original_image_view(unsigned int time)
{
	return wrap_raw_image_data(_sequence.frame(time));
}


void Hull::active_view_changed(const Descriptor &active_view)
{
	if (_active_view != active_view) {
		_active_view = active_view;

		update_image_control(_current_time);
	}
}


void Hull::update_fitting()
{
	if (_current_fitting) {
		_current_fitting->rig->deactivate();
		_current_fitting->proxy->disable();
		if (_connection_interaction_manager_signal_ui_updated.connected()) {
			_connection_interaction_manager_signal_ui_updated.disconnect();
		}
		_ui.clear_placeholders();
		_ui.image_control->drop_layer_manager();
	}

	_current_fitting = _ui.get_fitting();
	_current_fitting->proxy->enable();
	_current_fitting->rig->activate();

	update_toolbar();
	_ui.refresh_placeholders();

	if (_current_fitting->layer_manager) {
		_current_fitting->layer_manager->set_current_time(_current_time);
		_ui.image_control->set_layer_manager(_current_fitting->layer_manager);
	}

	if (_current_fitting->interaction_manager) {
		_connection_interaction_manager_signal_ui_updated =
						_current_fitting->interaction_manager->signal_ui_updated().connect(
								sigc::mem_fun(*this, &Hull::update_toolbar) );
	}
}


void Hull::update_toolbar()
{
	if (_current_fitting->interaction_manager) {
		// clear placeholder
		vector<Gtk::Widget* > children = _ui.left_side_layout->get_children();
		vector<Gtk::Widget* >::iterator it;
		for(it = children.begin(); it != children.end(); ++it) {
			_ui.left_side_layout->remove(**it);
		}

		Gtk::Box* toolbar = _current_fitting->interaction_manager->get_ui();
		if (toolbar) {
			_ui.left_side_layout->pack_start(*toolbar, Gtk::PACK_EXPAND_WIDGET);
		}
	}
}

/* private */
void Hull::refresh_view_menu()
{
	vector<ViewInfo> view_infos;
	std::map<Descriptor, View* >::iterator it;
	for (it = _view_map.begin(); it != _view_map.end(); ++it) {
		view_infos.push_back(ViewInfo(it->second->get_title(), it->first, it->second->get_position()));
	}

	// sort according to the position property
	std::sort(view_infos.begin(), view_infos.end());

	_ui.update_veiw_menu(view_infos, _active_view);
}


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
Glib::RefPtr<Gdk::Pixbuf> Hull::wrap_raw_image_data(const ImageFx<float> &image)
{
	const int BITS_PER_CHANNEL = 8;

	int size_x = image.size_x();
	int size_y = image.size_y();
    const float *image_data = image.raw();

	Glib::RefPtr<Gdk::Pixbuf> buffer = Gdk::Pixbuf::create(Gdk::COLORSPACE_RGB, false, BITS_PER_CHANNEL, size_x, size_y);
    /// NOTE: 'rowstride' is the length of internal representation of a row
    /// inside the Pixbuf, and it could differ from size_x*number_of_channels.
	int rowstride = buffer->get_rowstride();	// get internal row length
	int number_of_channels = buffer->get_n_channels();
	guint8* buffer_data = buffer->get_pixels();

    if (image.number_of_channels() == number_of_channels) {
        for (int y = 0; y < size_y; y++) {
            for (int x = 0; x < size_x; x++) {
                for (int c = 0; c < number_of_channels; c++) {
                    int image_index = number_of_channels * (size_x * y + x) + c;
                    char intensity = (unsigned char) image_data[image_index];
                    int data_index = y * rowstride + x * number_of_channels + c;
                    buffer_data[data_index] = intensity;
                }
            }
        }
    } else {    // NOTE: also handles the case of mono-channeled image
        for (int y = 0; y < size_y; y++) {
            for (int x = 0; x < size_x; x++) {
                int image_index = image.number_of_channels() * (size_x * y + x);    // take first channel
                char intensity = (unsigned char) image_data[image_index];
                for (int c = 0; c < number_of_channels; c++) {
                    int data_index = y * rowstride + x * number_of_channels + c;
                    buffer_data[data_index] = intensity;
                }
            }
        }
    }

	return buffer;
}


void Hull::update_image_control(int current_time)
{
	// ?TODO: receive Descriptor as parameter
	if (!_sequence.is_empty() && _view_map.find(_active_view) != _view_map.end()) {
		View *view = _view_map[_active_view];

		Glib::RefPtr<Gdk::Pixbuf> buffer = view->get_pixbuf(current_time);

		// check buffer and replace it with default one, if needed
		if (!buffer || buffer->get_width() != _sequence.size_x() || buffer->get_height() != _sequence.size_y()) {
			buffer = create_empty_pixbuf(_sequence.size_x(), _sequence.size_y());
		}

		_ui.image_control->set_pixbuf(buffer);
		_ui.image_control->queue_draw();
	}
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
