/*
 * geodesic_distance_ui.cpp
 *
 *  Created on: Jan 9, 2013
 *      Author: Vadim Fedorov
 */

#include "geodesic_distance_ui.h"

/* Public */

Geodesic_Distance_UI::Geodesic_Distance_UI()
{
	_ui.setup_ui(this);

	_ui.quit_action->signal_activate().connect( sigc::ptr_fun(&Gtk::Main::quit) );
	_ui.open_image_action->signal_activate().connect( sigc::mem_fun(*this, &Geodesic_Distance_UI::open_image) );
	_ui.open_sequence_action->signal_activate().connect( sigc::mem_fun(*this, &Geodesic_Distance_UI::open_sequence) );
	_ui.calculate_optical_flow_action->signal_activate().connect( sigc::mem_fun(*this, &Geodesic_Distance_UI::begin_calculate_optical_flow) );
	_ui.restore_optical_flow_action->signal_activate().connect( sigc::mem_fun(*this, &Geodesic_Distance_UI::restore_optical_flow) );
	_ui.toggle_optical_flow_action->signal_toggled().connect( sigc::mem_fun(*this, &Geodesic_Distance_UI::show_hide_optical_flow) );
	_ui.background_work_infobar->signal_response().connect( sigc::mem_fun(*this, &Geodesic_Distance_UI::perceive_background_worker) );

	_ui.patch_zoom_slider->signal_value_changed().connect( sigc::mem_fun(*this, &Geodesic_Distance_UI::set_patch_zoom) );
	_ui.distance_mode_picker->signal_changed().connect( sigc::mem_fun(*this, &Geodesic_Distance_UI::set_distance_mode) );
	_ui.motion_compensation_picker->signal_changed().connect( sigc::mem_fun(*this, &Geodesic_Distance_UI::set_motion_compensation_mode) );
	_ui.patch_size_picker->signal_changed().connect( sigc::mem_fun(*this, &Geodesic_Distance_UI::set_patch_size) );
	_ui.patch_duration_picker->signal_changed().connect( sigc::mem_fun(*this, &Geodesic_Distance_UI::set_patch_duration) );
	_ui.image_control->signal_point_selected().connect( sigc::mem_fun(*this, &Geodesic_Distance_UI::set_patch_location) );
	_ui.distance_weight_slider->signal_value_changed().connect( sigc::mem_fun(*this, &Geodesic_Distance_UI::set_distance_weight) );
	_ui.color_weight_slider->signal_value_changed().connect( sigc::mem_fun(*this, &Geodesic_Distance_UI::set_color_weight) );
	_ui.gamma_slider->signal_value_changed().connect( sigc::mem_fun(*this, &Geodesic_Distance_UI::set_gamma) );
	_ui.time_slider->signal_value_changed().connect( sigc::mem_fun(*this, &Geodesic_Distance_UI::set_time) );

	this->signal_key_press_event().connect( sigc::mem_fun(*this, &Geodesic_Distance_UI::move_selected_point) );

	// Note: if there are several different works to compute in background, recreate dispatchers and connect to them in 'begin_...' methods
	_work_done_dispatcher.connect( sigc::mem_fun(*this, &Geodesic_Distance_UI::end_calculate_optical_flow) );
	_portion_ready_dispatcher.connect( sigc::mem_fun(*this, &Geodesic_Distance_UI::take_optical_flow_frame) );

	_sequence = 0;
	_distances = 0;
	_patch_scale = 1;

	_ui.background_work_infobar->hide();
	_ui.optical_flow_action_group->set_sensitive(false);
	_ui.patch_control->set_redraw_on_allocate(false);
	_ui.time_slider->set_sensitive(false);
	_ui.patch_duration_picker->set_sensitive(false);

	_ui.patch_zoom_slider->set_range(1, MAX_PATCH_SCALE);
	_ui.patch_zoom_slider->set_digits(0);

	_ui.distance_mode_picker->append("patch_limit", "Use only patch points");
	_ui.distance_mode_picker->append("extended_limit", "Use extended range (2x)");
	_ui.distance_mode_picker->set_active(0);

	_ui.motion_compensation_picker->append("no", "No tracking");
	_ui.motion_compensation_picker->append("pixelwise", "Pixelwise tracking");
	_ui.motion_compensation_picker->set_active(0);

	for ( int i=5; i<=15; i+=2 ) {
		_ui.patch_size_picker->append(Glib::ustring::compose("%1",i));
	}
	_ui.patch_size_picker->set_active(1);				//_patch_size = 7

	for ( int i=1; i<=7; i+=2 ) {
		_ui.patch_duration_picker->append(Glib::ustring::compose("%1",i));
	}
	_ui.patch_duration_picker->set_active(0);			//_patch_duration = 1

	_ui.distance_weight_slider->set_range(0, 1.0);
	_ui.distance_weight_slider->set_value(0.1);
	_ui.color_weight_slider->set_range(0, 1.0);
	_ui.color_weight_slider->set_value(0.1);
	_ui.gamma_slider->set_range(0.01, 0.2);
	_ui.gamma_slider->set_digits(2);
	_ui.gamma_slider->set_value(0.09);
}

Geodesic_Distance_UI::~Geodesic_Distance_UI()
{

}


/* slots */
void Geodesic_Distance_UI::open_image()
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

		Image *image = ReadPgmImage(&filename);
		// TODO: check image

		// Adjust UI
		_ui.optical_flow_action_group->set_sensitive(false);
		_ui.patch_duration_picker->set_active(0);
		_ui.patch_duration_picker->set_sensitive(false);
		_ui.time_slider->set_sensitive(false);
		_ui.time_slider->set_range(0, 0);

		// Set initial coordinates
		_patch_center.x = image->GetXSize() / 2;
		_patch_center.y = image->GetYSize() / 2;
		_patch_center.t = 0;
		_current_time = 0;
		UpdateCoordinates();

		// Replace single image with sequence of the only element for computational uniformity.
		_sequence = new Sequence(image);

		// Show image
		update_image_control(_current_time);

		// Get patch, then color representation of distances, then pixmap for the only time point and finally show it
		Shape patch_size = Shape(_patch_size, _patch_size, _patch_duration);
		_distances = calculate_distances(*_sequence,_patch_center, patch_size, 0, _distance_mode, _distance_weight, _color_weight);
		_color_representations = draw_distances_with_color(*_distances, _gamma);
		_patch_slice = _color_representations[0];
		ShowPatchPixbuf(_patch_slice, _patch_scale);
	}
}


void Geodesic_Distance_UI::open_sequence()
{
	Gtk::FileChooserDialog dialog("Please choose a sequence folder", Gtk::FILE_CHOOSER_ACTION_SELECT_FOLDER);
	dialog.set_transient_for(*this);

	//Add response buttons the the dialog:
	dialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
	dialog.add_button("Select", Gtk::RESPONSE_OK);

	int result = dialog.run();

	if (result == Gtk::RESPONSE_OK) {
		std::string sequence_folder = dialog.get_filename();
		sequence_folder = sequence_folder.append("/");

		// Get the list of frame names.
		Glib::RefPtr<Gio::File> dir = Gio::File::create_for_path(sequence_folder);
		Glib::RefPtr<Gio::FileEnumerator> child_enumeration = dir->enumerate_children();
		Glib::RefPtr<Gio::FileInfo> file_info;
		std::vector<Glib::ustring> file_names;
		while ((file_info = child_enumeration->next_file()) != NULL)
		{
			std::string file_mime_type = file_info->get_content_type();
			if (file_mime_type.compare("image/x-portable-graymap") == 0) {
				std::string file_name = file_info->get_name();
				file_names.push_back(file_name);
			}
		}

		if (file_names.size() == 0)
			return;

		// Sort filenames in ascending order using default < operation
		std::sort(file_names.begin(), file_names.end());

		// Load first frame, that will define width and height for the sequence.
		std::string first_frame_path = sequence_folder;
		first_frame_path.append(file_names[0]);
		Image *first_frame = ReadPgmImage(&first_frame_path);

		// [Re]create sequence instance
		if (_sequence)
			delete _sequence;
		_sequence = new Sequence(first_frame);

		// Load the rest frames.
		for (int i=1; i<file_names.size(); i++) {
			string frame_path = sequence_folder;
			frame_path.append(file_names[i]);
			Image *frame = ReadPgmImage(&frame_path);
			_sequence->AddFrame(frame);
		}

		// Adjust UI
		_ui.optical_flow_action_group->set_sensitive(true);
		_ui.patch_duration_picker->set_active(0);
		_ui.patch_duration_picker->set_sensitive(true);
		_ui.time_slider->set_sensitive(true);
		_ui.time_slider->set_range(0, _sequence->GetTSize() - 1);
		_ui.time_slider->set_digits(0);
		std::string optical_flow_file_name = sequence_folder;
		optical_flow_file_name.append("optical_flow_data");
		bool is_optical_flow_calculated =
				check_optical_flow(optical_flow_file_name, _sequence->GetXSize(), _sequence->GetYSize(), _sequence->GetTSize() - 1);
		_ui.restore_optical_flow_action->set_sensitive(is_optical_flow_calculated);
		// NOTE: no auto restore, so menu item could be shaded and motion compensation denied
		_ui.toggle_optical_flow_action->set_sensitive(false);
		_ui.motion_compensation_picker->set_sensitive(false);

		// Set initial coordinates
		_sequence_folder = sequence_folder;
		_patch_center.x = first_frame->GetXSize() / 2;
		_patch_center.y = first_frame->GetYSize() / 2;
		_patch_center.t = 0;
		_current_time = 0;
		_optical_flow_list.clear();
		_optical_flow_list = std::vector<OpticalFlowContainer*>(_sequence->GetTSize() - 1);
		for (int i = 0; i < _optical_flow_list.size(); i++) {
			_optical_flow_list[i] = new OpticalFlow();
		}
		UpdateCoordinates();

		// Show first frame
		update_image_control(_current_time);

		// Get patch, then color representation of distances, then pixmap for current time point and finally show it
		Shape patch_size = Shape(_patch_size, _patch_size, _patch_duration);
		_distances = calculate_distances(*_sequence,_patch_center, patch_size, 0, _distance_mode, _distance_weight, _color_weight);
		_color_representations = draw_distances_with_color(*_distances, _gamma);
		_patch_slice = GetDistanceRepresentatonByTime(_color_representations, _patch_center.t, _current_time, _empty_pixmap);
		ShowPatchPixbuf(_patch_slice, _patch_scale);
	}
}


void Geodesic_Distance_UI::set_distance_mode()
{
	// get new distance mode value
	Glib::ustring id = _ui.distance_mode_picker->get_active_id();
	DistanceMode mode;
	if (id == "extended_limit") {
		mode = extended_space;
	} else if (id == "patch_limit") {
		mode = patch_space;
	} else {
		mode = patch_space;
	}

	// check if new and old value differs
	if (mode != _distance_mode) {
		_distance_mode = mode;
	} else {
		return;
	}

	// update distances using new distance mode
	if (_sequence) {
		Shape patch_size = Shape(_patch_size, _patch_size, _patch_duration);
		_distances = calculate_distances(*_sequence,_patch_center, patch_size, 0, _distance_mode, _distance_weight, _color_weight);
		_color_representations = draw_distances_with_color(*_distances, _gamma);
		_patch_slice = GetDistanceRepresentatonByTime(_color_representations, _patch_center.t, _current_time, _empty_pixmap);
		ShowPatchPixbuf(_patch_slice, _patch_scale);
	}
}


void Geodesic_Distance_UI::set_motion_compensation_mode()
{
	// get new motion compensation mode value
	Glib::ustring id = _ui.motion_compensation_picker->get_active_id();
	MotionCompensationMode mode;
	if (id == "no") {
		mode = no_compensation;
	} else if (id == "pixelwise") {
		mode = pixelwise;
	} else {
		mode = no_compensation;
	}

	// check if new and old value differs
	if (mode != _motion_compensation_mode) {
		_motion_compensation_mode = mode;
	} else {
		return;
	}

	//TODO: update using new motion compensation mode
	/*if (_motion_compensation_mode == pixelwise) {
		Sequence* compensated = _sequence->compensate_motion_pixelwise(_optical_flow_list, _current_time, _current_time + 1, false);
		Glib::RefPtr<Gdk::Pixbuf> buffer = WrapRawImageData(compensated->GetFrame(_current_time));
		_ui.image_control->set_pixbuf(buffer);
		_ui.image_control->queue_draw();
	}*/
	if (_sequence) {
		Shape patch_size = Shape(_patch_size, _patch_size, _patch_duration);
		_distances = calculate_distances(*_sequence,_patch_center, patch_size, 0, _distance_mode, _distance_weight, _color_weight);
		_color_representations = draw_distances_with_color(*_distances, _gamma);
		_patch_slice = GetDistanceRepresentatonByTime(_color_representations, _patch_center.t, _current_time, _empty_pixmap);
		ShowPatchPixbuf(_patch_slice, _patch_scale);
	}
}


void Geodesic_Distance_UI::set_patch_zoom()
{
	_patch_scale = (int)_ui.patch_zoom_slider->get_value();
	ShowPatchPixbuf(_patch_slice, _patch_scale);
}


void Geodesic_Distance_UI::set_patch_size()
{
	Glib::ustring text_value = _ui.patch_size_picker->get_active_text();
	int int_value;

	// string to integer
	std::stringstream s;
	s << text_value.raw();
	s >> int_value;

	if (int_value == _patch_size)
		return;

	_patch_size = int_value;
	_empty_pixmap = CreateEmptyPixbuf(_patch_size, _patch_size);

	if (_sequence) {
		Shape patch_size = Shape(_patch_size, _patch_size, _patch_duration);
		_distances = calculate_distances(*_sequence,_patch_center, patch_size, 0, _distance_mode, _distance_weight, _color_weight);
		_color_representations = draw_distances_with_color(*_distances, _gamma);
		_patch_slice = GetDistanceRepresentatonByTime(_color_representations, _patch_center.t, _current_time, _empty_pixmap);
		ShowPatchPixbuf(_patch_slice, _patch_scale);
	}
}

/*
 * Set patch size in T direction (duration).
 */
void Geodesic_Distance_UI::set_patch_duration()
{
	Glib::ustring text_value = _ui.patch_duration_picker->get_active_text();
	int int_value;

	// string to integer
	std::stringstream s;
	s << text_value.raw();
	s >> int_value;

	if (int_value == _patch_duration)
		return;

	int patch_time_offset = _patch_center.t - (int)(int_value / 2);

	if (patch_time_offset < 0 || (_sequence && patch_time_offset + int_value >= _sequence->GetTSize() )) {
		_ui.patch_duration_picker->set_active(0);
		ShowStatusMessage("Patch must not exceed temporal borders.");
		return;
	}

	_patch_duration = int_value;

	if (_sequence) {
		Shape patch_size = Shape(_patch_size, _patch_size, _patch_duration);
		_distances = calculate_distances(*_sequence,_patch_center, patch_size, 0, _distance_mode, _distance_weight, _color_weight);
		_color_representations = draw_distances_with_color(*_distances, _gamma);
		_patch_slice = GetDistanceRepresentatonByTime(_color_representations, _patch_center.t, _current_time, _empty_pixmap);
		ShowPatchPixbuf(_patch_slice, _patch_scale);
	}
}


void Geodesic_Distance_UI::set_patch_location(int patch_center_x, int patch_center_y)
{
	int half_size = _patch_size / 2;

	if (_sequence) {
		if (patch_center_x - half_size < 0 ||
			patch_center_y - half_size < 0 ||
			patch_center_x + half_size >= _sequence->GetXSize() ||
			patch_center_y + half_size >= _sequence->GetYSize()) {
			ShowStatusMessage("Patch must not exceed spatial borders.");
			return;
		}

		_patch_center.x = patch_center_x;
		_patch_center.y = patch_center_y;
		_patch_center.t = _current_time;

		Shape patch_size = Shape(_patch_size, _patch_size, _patch_duration);
		_distances = calculate_distances(*_sequence,_patch_center, patch_size, 0, _distance_mode, _distance_weight, _color_weight);
		_color_representations = draw_distances_with_color(*_distances, _gamma);
		_patch_slice = GetDistanceRepresentatonByTime(_color_representations, _patch_center.t, _current_time, _empty_pixmap);
		ShowPatchPixbuf(_patch_slice, _patch_scale);

		UpdateCoordinates();
	}
}


void Geodesic_Distance_UI::set_distance_weight()
{
	_distance_weight =_ui.distance_weight_slider->get_value();

	if (_sequence) {
		Shape patch_size = Shape(_patch_size, _patch_size, _patch_duration);
		_distances = calculate_distances(*_sequence,_patch_center, patch_size, 0, _distance_mode, _distance_weight, _color_weight);
		_color_representations = draw_distances_with_color(*_distances, _gamma);
		_patch_slice = GetDistanceRepresentatonByTime(_color_representations, _patch_center.t, _current_time, _empty_pixmap);
		ShowPatchPixbuf(_patch_slice, _patch_scale);
	}
}


void Geodesic_Distance_UI::set_color_weight()
{
	_color_weight = _ui.color_weight_slider->get_value();

	if (_sequence) {
		Shape patch_size = Shape(_patch_size, _patch_size, _patch_duration);
		_distances = calculate_distances(*_sequence,_patch_center, patch_size, 0, _distance_mode, _distance_weight, _color_weight);
		_color_representations = draw_distances_with_color(*_distances, _gamma);
		_patch_slice = GetDistanceRepresentatonByTime(_color_representations, _patch_center.t, _current_time, _empty_pixmap);
		ShowPatchPixbuf(_patch_slice, _patch_scale);
	}
}


void Geodesic_Distance_UI::set_gamma()
{
	_gamma = _ui.gamma_slider->get_value();

	if (_distances) {
		_color_representations = draw_distances_with_color(*_distances, _gamma);
		_patch_slice = GetDistanceRepresentatonByTime(_color_representations, _patch_center.t, _current_time, _empty_pixmap);
		ShowPatchPixbuf(_patch_slice, _patch_scale);
	}
}


void Geodesic_Distance_UI::set_time()
{
	_current_time = _ui.time_slider->get_value();

	update_image_control(_current_time);

	if (_color_representations.size() > 0) {
		_patch_slice = GetDistanceRepresentatonByTime(_color_representations, _patch_center.t, _current_time, _empty_pixmap);
		ShowPatchPixbuf(_patch_slice, _patch_scale);
	}
}


bool Geodesic_Distance_UI::move_selected_point(GdkEventKey* event)
{
	std::string key = event->string;

	if (key.compare("w") == 0) {
		if (_sequence && _patch_center.y > 1)
			set_patch_location(_patch_center.x, _patch_center.y - 1);
	}
	if (key.compare("a") == 0) {
		if (_sequence && _patch_center.x > 1)
			set_patch_location(_patch_center.x - 1, _patch_center.y);
	}
	if (key.compare("s") == 0) {
		if (_sequence && _patch_center.y < _sequence->GetYSize() - 1)
			set_patch_location(_patch_center.x, _patch_center.y + 1);
	}
	if (key.compare("d") == 0) {
		if (_sequence && _patch_center.x < _sequence->GetXSize() - 1)
			set_patch_location(_patch_center.x + 1, _patch_center.y);
	}

	return false;
}


void Geodesic_Distance_UI::perceive_background_worker(int responce_id)
{
	if (responce_id == 0)
		cancel_calculate_optical_flow();
}


void Geodesic_Distance_UI::store_optical_flow(OpticalFlowContainer &flow, int index)
{
	if (_sequence_folder.empty())
		return;

	std::string file_name = _sequence_folder;
	file_name.append("optical_flow_data");

	int chunks_count = _sequence->GetTSize() - 1;
	//int x_size = _sequence->GetXSize();
	//int y_size = _sequence->GetYSize();
	//float *flow_x = flow.get_flow_x();
	//float *flow_y = flow.get_flow_y();

	//update_or_overwrite_flow(file_name, flow_x, flow_y, x_size, y_size, index, chunks_count);
	update_or_overwrite_flow(file_name, flow, index, chunks_count);
}

void Geodesic_Distance_UI::restore_optical_flow()
{
	if (_sequence_folder.empty())
		return;

	std::string file_name = _sequence_folder;
	file_name.append("optical_flow_data");
	int chunks_count = _sequence->GetTSize() - 1;
	int size_x;
	int size_y;
	float* buf_x;
	float* buf_y;
	bool has_some_data = false;

	// TODO: add method for reading all chunks at a time and use it here
	for (int i = 0; i <  chunks_count; i++) {
		if (read_flow(file_name, buf_x, buf_y, size_x, size_y, i)) {
			if (size_x ==_sequence->GetXSize() && size_y == _sequence->GetYSize()) {
				_optical_flow_list[i]->set_flow(buf_x, buf_y, size_x, size_y);
				has_some_data = true;
			}
		}
	}

	if (has_some_data) {
		_ui.toggle_optical_flow_action->set_sensitive(true);
		_ui.motion_compensation_picker->set_sensitive(true);
	}

}


void Geodesic_Distance_UI::show_hide_optical_flow()
{
	update_image_control(_current_time);
}

/* private */
int Geodesic_Distance_UI::write_flow(float *u, float *v, int w, int h)
{
	FILE *fp;
	int i, j, offset;

	fp = fopen("flow.data", "w");

	offset = 0;

	for(i = 0; i < h; i++) {
		for(j = 0; j < w; j++, offset++) {
			fprintf(fp, "%d %d %f %f\n", j, h - 1 - i, u[offset], -v[offset]);
		}
		fprintf(fp, "\n");
	}

	fclose(fp);

	return 1;
}

/**
 * Converts Image into Gdk::Pixbuf
 */
Glib::RefPtr<Gdk::Pixbuf> Geodesic_Distance_UI::WrapRawImageData(Image *image)
{
	const int BITS_PER_CHANNEL = 8;

	int x_size = image->GetXSize();
	int y_size = image->GetYSize();

	Glib::RefPtr<Gdk::Pixbuf> buffer = Gdk::Pixbuf::create(Gdk::COLORSPACE_RGB, false, BITS_PER_CHANNEL, x_size, y_size);
	int rowstride = buffer->get_rowstride();	// get internal row length
	int number_of_channels = buffer->get_n_channels();
	guint8* data = buffer->get_pixels();

	int index;
	for (int y = 0;y < y_size;y++) {
		for (int x = 0;x < x_size;x++) {
			char intensity = (unsigned char)image->GetPixelValue(x, y);
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


Sequence* Geodesic_Distance_UI::calculate_distances( Sequence &sequence,
													const Point &patch_center,
													const Shape &patch_size,
													float distance_threshold,
													DistanceMode mode,
													float distance_weight,
													float color_weight)
{
	//TODO: use refs instead of pointers
	IDistanceCalculation *distanceCalc = new SimpleDistanceCalculation(distance_weight, color_weight);
	DijkstraAlgorithm marching = DijkstraAlgorithm(distanceCalc);

	//TODO: rewrite
	Sequence *distances = 0;
	switch(mode) {
		case extended_space: {
			float factor = 2.0;	// Note: hardcoded for the moment
			Shape lookup_limits;
			lookup_limits.size_x = patch_size.size_x * factor;
			lookup_limits.size_y = patch_size.size_y * factor;
			lookup_limits.size_t = patch_size.size_t * factor;
			if (_motion_compensation_mode == pixelwise) {
				distances = marching.CalculateDistance(sequence, _optical_flow_list, lookup_limits, patch_size, patch_center);
			} else {
				distances = marching.CalculateDistance(sequence, lookup_limits, patch_size, patch_center);
			}
			break;
		}
		case patch_space: {
			// TODO: maybe get patch first
			if (_motion_compensation_mode == pixelwise) {
				distances = marching.CalculateDistance(sequence, _optical_flow_list, patch_size, patch_size, patch_center);
			} else {
				distances = marching.CalculateDistance(sequence, patch_size, patch_size, patch_center);
			}
			break;
		}
		case threshold: {
			distances = marching.CalculateDistance(sequence);
			// TODO: implement. Use some kind of crop
			break;
		}
	}

	return distances;
}


vector<Glib::RefPtr<Gdk::Pixbuf> > Geodesic_Distance_UI::draw_distances_with_color(Sequence &distances, float gamma)
{
	DijkstraAlgorithm marching = DijkstraAlgorithm(0);

	Sequence *distanceRepresentation =
				marching.BuildColorDistanceRepresentation(distances, gamma);

	int patch_duration = distances.GetTSize();
	vector<Glib::RefPtr<Gdk::Pixbuf> > color_representations = vector<Glib::RefPtr<Gdk::Pixbuf> >(patch_duration);
	for (int i=0; i<patch_duration; i++) {
		color_representations[i] = WrapRawImageData(distanceRepresentation->GetFrame(i));
	}

	delete distanceRepresentation;

	return color_representations;
}


/*
 * From colored representation selects one for corresponding time point
 */
Glib::RefPtr<Gdk::Pixbuf> Geodesic_Distance_UI::GetDistanceRepresentatonByTime(vector<Glib::RefPtr<Gdk::Pixbuf> > representation, int patch_time_center, int current_time, Glib::RefPtr<Gdk::Pixbuf> empty_pixbuf)
{
	int patch_duration = representation.size();
	int patch_time = current_time - patch_time_center + (int)(patch_duration / 2);


	Glib::RefPtr<Gdk::Pixbuf> patch_pixbuf;
	if (patch_time < 0 || patch_time >= patch_duration) {
		patch_pixbuf = empty_pixbuf;
	} else {
		patch_pixbuf = representation[patch_time];
	}

	return patch_pixbuf;
}


void Geodesic_Distance_UI::ShowPatchPixbuf(Glib::RefPtr<Gdk::Pixbuf> pixbuf, int scale)
{
	if (scale < 1)
		return;

	if (scale > 1) {
		int width = pixbuf->get_width() * scale;
		int height = pixbuf->get_width() * scale;
		pixbuf = pixbuf->scale_simple(width, height, Gdk::INTERP_NEAREST);
	}

	_ui.patch_control->set_pixbuf(pixbuf);
	_ui.patch_control->queue_draw();
}


void Geodesic_Distance_UI::update_image_control(int current_time)
{
	bool show_optical_flow = _ui.toggle_optical_flow_action->get_active();
	Glib::RefPtr<Gdk::Pixbuf> buffer;

	if (show_optical_flow && _optical_flow_list.size() > 0) {
		if (current_time < _optical_flow_list.size() && _optical_flow_list[current_time]->contains_data()) {
			buffer = static_cast<OpticalFlow*>(_optical_flow_list[current_time])->get_color_code_view();
		} else {
			buffer = CreateEmptyPixbuf(_sequence->GetXSize(), _sequence->GetYSize());
		}
	} else {
		buffer = WrapRawImageData(_sequence->GetFrame(current_time));
	}

	_ui.image_control->set_pixbuf(buffer);
	_ui.image_control->queue_draw();
}


Glib::RefPtr<Gdk::Pixbuf> Geodesic_Distance_UI::CreateEmptyPixbuf(int width, int height)
{
	//todo: check
	Glib::RefPtr<Gdk::Pixbuf> empty_image = Gdk::Pixbuf::create(Gdk::COLORSPACE_RGB, false, 8, width, height);
	empty_image->fill(0);
	return empty_image;
}

void Geodesic_Distance_UI::ShowStatusMessage(std::string message)
{
	_ui.status_bar->pop(0);
	_ui.status_bar->push(message, 0);
}

void Geodesic_Distance_UI::UpdateCoordinates()
{
	Glib::ustring coordinates = Glib::ustring::compose("x: %1; y: %2; t: %3", _patch_center.x, _patch_center.y, _patch_center.t);
	_ui.coordinates_label->set_label(coordinates);
}


void Geodesic_Distance_UI::begin_calculate_optical_flow()
{
	//TODO: ensure no background operations in progress.

	_aux_stop_background_work_flag = false;
	_progress_counter = 0;

	try
	{
		// NOTE: if specify no tasks, optical flow for all frame pairs will be computed
		std::vector<int> empty_task_list;
		_background_worker = Glib::Threads::Thread::create( sigc::bind<std::vector<int> >( sigc::mem_fun(*this, &Geodesic_Distance_UI::calculate_optical_flow), empty_task_list));
		//_background_worker = Glib::Threads::Thread::create( sigc::mem_fun(*this, &Geodesic_Distance_UI::calculate_optical_flow));
	}
	catch(Glib::Threads::ThreadError &err)
	{
		//TODO: notify user abour error and log
		return;
	}

	Glib::ustring message = Glib::ustring::compose("Optical flow. Frames finished: %1; frames left: %2.", 0, _sequence->GetTSize() - 1);
	_ui.background_work_infobar_message->set_text(message);
	_ui.calculate_optical_flow_action->set_sensitive(false);
	_ui.background_work_infobar->show();
}


void Geodesic_Distance_UI::end_calculate_optical_flow()
{
	_background_worker->join();

	_ui.background_work_infobar->hide();
	_ui.calculate_optical_flow_action->set_sensitive(true);
}


void Geodesic_Distance_UI::take_optical_flow_frame()
{
	int size_x = _sequence->GetXSize();
	int size_y = _sequence->GetYSize();
	int index;

	// Take calculated values
	{
		Glib::Threads::Mutex::Lock lock(_background_work_mutex);
		index = _aux_optical_flow_index;
		_optical_flow_list[index]->set_flow(_aux_optical_flow_x, _aux_optical_flow_y, size_x, size_y);
	}

	_progress_counter++;
	Glib::ustring message = Glib::ustring::compose("Optical flow. Frames finished: %1; frames left: %2.", _progress_counter, _sequence->GetTSize() - _progress_counter - 1);
	_ui.background_work_infobar_message->set_text(message);

	_ui.toggle_optical_flow_action->set_sensitive(true);
	_ui.motion_compensation_picker->set_sensitive(true);

	store_optical_flow(*_optical_flow_list[index], index);

	_ui.restore_optical_flow_action->set_sensitive(true);
}


void Geodesic_Distance_UI::cancel_calculate_optical_flow()
{
	_ui.background_work_infobar_message->set_text("Optical flow. Canceling... ");
	{
		Glib::Threads::Mutex::Lock lock(_background_work_mutex);
		_aux_stop_background_work_flag = true;
	}
}


bool Geodesic_Distance_UI::allow_background_computation()
{
	bool stop_flag;

	{
		Glib::Threads::Mutex::Lock lock(_background_work_mutex);
		stop_flag = _aux_stop_background_work_flag;
	}

	return !stop_flag;
}


/******************************************************************************************************
 * If pass no tasks, forward and backward optical flow fields for all pairs of frames will be computed.
 * In task list absolute value defines from what frame to compute flow, and sign defines direction.
 * Example: '0' - forward flow from frame 0 to frame 1; '-2' - backward flow from frame 2 to frame 1.
 */
void Geodesic_Distance_UI::calculate_optical_flow(std::vector<int> task_list)
{
	if (!_sequence)
		return;

	// Prepare watchdog for catching possible cancellation command from user
	SignalWatchdog *watchdog = new SignalWatchdog();
	watchdog->signal_ask_permission().connect( sigc::mem_fun(*this, &Geodesic_Distance_UI::allow_background_computation ) );

	// Create with default parameters
	Zach_TVL1_OpticalFlow* opticalFlow = new Zach_TVL1_OpticalFlow(false);
	opticalFlow->set_watchdog(watchdog);

	int x_size = _sequence->GetXSize();
	int y_size = _sequence->GetYSize();
	int frame_count = _sequence->GetTSize();

	// Fill task list, if empty
	if (task_list.size() == 0) {
		int flow_count = frame_count - 1;	// in one direction
		task_list.reserve(flow_count /** 2*/);

		for (int i = 0; i < flow_count; i++)
			task_list.push_back(i);
		/*for (int i = -1; i >= -flow_count; i--)
			task_list.push_back(i);*/
	}

	std::vector<int>::iterator it;
	for (it = task_list.begin(); it != task_list.end(); ++it) {
		// Get frames data
		int first_index = std::abs(*it);
		int second_index = (*it >= 0)? first_index + 1 : first_index - 1;
		float* first_frame_data = _sequence->GetFrame(first_index)->GetRawData();
		float* second_frame_data = _sequence->GetFrame(second_index)->GetRawData();

		// Allocate memory for the flow
		float *u1 = new float[x_size * y_size];
		float *u2 = new float[x_size * y_size];

		// Calculate optical flow
		//me_prefilter(first_frame_data, second_frame_data, x_size, y_size);
		bool success = opticalFlow->calculate_with_multiscale(first_frame_data, second_frame_data, u1, u2, x_size, y_size);

		if (success) {
			// Set calculated values
			{
				Glib::Threads::Mutex::Lock lock(_background_work_mutex);
				_aux_optical_flow_x = u1;
				_aux_optical_flow_y = u2;
				_aux_optical_flow_index = *it;
			}

			// Notify main thread, that i-th optical flow slice is calculated
			_portion_ready_dispatcher();
		}

		// Check stop flag
		if (!allow_background_computation()) {
			break;
		}
	}

	// Notify main thread, that the background work is done
	_work_done_dispatcher();
}



