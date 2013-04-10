/*
 * patch_weight_mode.cpp
 *
 *  Created on: Apr 9, 2013
 *      Author: Vadim Fedorov
 */

#include "patch_weight_rig.h"

Patch_Weight_Rig::Patch_Weight_Rig(IHull *hull)
{
	_hull = hull;
	_ui = Patch_Weight_UI_Container();

	// Connect to ui signals.
	_ui.patch_zoom_slider->signal_value_changed().connect( sigc::mem_fun(*this, &Patch_Weight_Rig::set_patch_zoom) );
	_ui.distance_mode_picker->signal_changed().connect( sigc::mem_fun(*this, &Patch_Weight_Rig::set_distance_mode) );
	_ui.motion_compensation_picker->signal_changed().connect( sigc::mem_fun(*this, &Patch_Weight_Rig::set_motion_compensation_mode) );
	_ui.patch_size_picker->signal_changed().connect( sigc::mem_fun(*this, &Patch_Weight_Rig::set_patch_size) );
	_ui.patch_duration_picker->signal_changed().connect( sigc::mem_fun(*this, &Patch_Weight_Rig::set_patch_duration) );
	_ui.distance_weight_slider->signal_value_changed().connect( sigc::mem_fun(*this, &Patch_Weight_Rig::set_distance_weight) );
	_ui.color_weight_slider->signal_value_changed().connect( sigc::mem_fun(*this, &Patch_Weight_Rig::set_color_weight) );
	_ui.gamma_slider->signal_value_changed().connect( sigc::mem_fun(*this, &Patch_Weight_Rig::set_gamma) );
}


void Patch_Weight_Rig::activate()
{
	// TODO: check if ui is been built
	// TODO: assign ui to some placeholder
	// TODO: check if sequence is the same to restore some data from previous session
	Sequence* sequence = _hull->request_sequence();
	int current_time = _hull->request_current_time();

	// Set initial coordinates
	_patch_center.x = sequence->GetXSize() / 2;
	_patch_center.y = sequence->GetYSize() / 2;
	_patch_center.t = 0;
	_patch_duration = 1;
	update_coordinates();

	// Get patch, then color representation of distances
	Shape patch_size = Shape(_patch_size, _patch_size, _patch_duration);
	_distances = calculate_distances(*sequence,_patch_center, patch_size, 0, _distance_mode, _distance_weight, _color_weight);
	_color_representations = draw_distances_with_color(*_distances, _gamma);

	if (sequence->GetTSize() > 1) {
		// Adjust UI for a video sequence
		_ui.patch_duration_picker->set_active(0);
		_ui.patch_duration_picker->set_sensitive(true);
		_ui.motion_compensation_picker->set_sensitive(false);

		// Get pixmap for current time point
		_patch_slice = get_distance_representaton_by_time(_color_representations, _patch_center.t, current_time, _empty_pixmap);

	} else {
		// Adjust UI for a stil image
		_ui.patch_duration_picker->set_active(0);
		_ui.patch_duration_picker->set_sensitive(false);

		// Get pixmap for the only time point.
		_patch_slice = _color_representations[0];
	}

	// Show patch
	show_patch_pixbuf(_patch_slice, _patch_scale);
}


void Patch_Weight_Rig::deactivate()
{

}


/* Private Slots */

void Patch_Weight_Rig::set_distance_mode()
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

	Sequence* sequence = _hull->request_sequence();
	int current_time = _hull->request_current_time();

	// update distances using new distance mode
	if (sequence) {
		Shape patch_size = Shape(_patch_size, _patch_size, _patch_duration);
		_distances = calculate_distances(*sequence,_patch_center, patch_size, 0, _distance_mode, _distance_weight, _color_weight);
		_color_representations = draw_distances_with_color(*_distances, _gamma);
		_patch_slice = get_distance_representaton_by_time(_color_representations, _patch_center.t, current_time, _empty_pixmap);
		show_patch_pixbuf(_patch_slice, _patch_scale);
	}
}


void Patch_Weight_Rig::set_motion_compensation_mode()
{
	// get new motion compensation mode value
	Glib::ustring id = _ui.motion_compensation_picker->get_active_id();
	MotionCompensationMode mode;
	if (id == "no") {
		mode = no_compensation;
	} else if (id == "pixelwise") {
		mode = pixelwise;
	} else if (id == "shift_central") {
		mode = patch_shift_central;
	} else if (id == "shift_weighted") {
		mode = patch_shift_weighted;
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

	Sequence* sequence = _hull->request_sequence();
	int current_time = _hull->request_current_time();

	if (sequence) {
		Shape patch_size = Shape(_patch_size, _patch_size, _patch_duration);
		_distances = calculate_distances(*sequence,_patch_center, patch_size, 0, _distance_mode, _distance_weight, _color_weight);
		_color_representations = draw_distances_with_color(*_distances, _gamma);
		_patch_slice = get_distance_representaton_by_time(_color_representations, _patch_center.t, current_time, _empty_pixmap);
		show_patch_pixbuf(_patch_slice, _patch_scale);
	}
}


void Patch_Weight_Rig::set_patch_zoom()
{
	_patch_scale = (int)_ui.patch_zoom_slider->get_value();
	show_patch_pixbuf(_patch_slice, _patch_scale);
}


void Patch_Weight_Rig::set_patch_size()
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

	Sequence* sequence = _hull->request_sequence();
	int current_time = _hull->request_current_time();

	if (sequence) {
		Shape patch_size = Shape(_patch_size, _patch_size, _patch_duration);
		_distances = calculate_distances(*sequence,_patch_center, patch_size, 0, _distance_mode, _distance_weight, _color_weight);
		_color_representations = draw_distances_with_color(*_distances, _gamma);
		_patch_slice = get_distance_representaton_by_time(_color_representations, _patch_center.t, current_time, _empty_pixmap);
		show_patch_pixbuf(_patch_slice, _patch_scale);
	}

	// Update layer.
	Patch_Position_Layer *layer = find_or_create_patch_position_layer(_layer_manager);
	layer->set_patch_size(_patch_size);
}


/*
 * Set patch size in T direction (duration).
 */
void Patch_Weight_Rig::set_patch_duration()
{
	Glib::ustring text_value = _ui.patch_duration_picker->get_active_text();
	int int_value;

	// string to integer
	std::stringstream s;
	s << text_value.raw();
	s >> int_value;

	if (int_value == _patch_duration)
		return;

	Sequence* sequence = _hull->request_sequence();
	int current_time = _hull->request_current_time();

	int patch_time_offset = _patch_center.t - (int)(int_value / 2);

	if (patch_time_offset < 0 || (sequence && patch_time_offset + int_value >= sequence->GetTSize() )) {
		_ui.patch_duration_picker->set_active(0);
		// TODO: request messaging service from the hull
		//ShowStatusMessage("Patch must not exceed temporal borders.");
		return;
	}

	_patch_duration = int_value;

	if (sequence) {
		Shape patch_size = Shape(_patch_size, _patch_size, _patch_duration);
		_distances = calculate_distances(*sequence,_patch_center, patch_size, 0, _distance_mode, _distance_weight, _color_weight);
		_color_representations = draw_distances_with_color(*_distances, _gamma);
		_patch_slice = get_distance_representaton_by_time(_color_representations, _patch_center.t, current_time, _empty_pixmap);
		show_patch_pixbuf(_patch_slice, _patch_scale);
	}
}


void Patch_Weight_Rig::set_distance_weight()
{
	_distance_weight =_ui.distance_weight_slider->get_value();

	Sequence* sequence = _hull->request_sequence();
	int current_time = _hull->request_current_time();

	if (sequence) {
		Shape patch_size = Shape(_patch_size, _patch_size, _patch_duration);
		_distances = calculate_distances(*sequence,_patch_center, patch_size, 0, _distance_mode, _distance_weight, _color_weight);
		_color_representations = draw_distances_with_color(*_distances, _gamma);
		_patch_slice = get_distance_representaton_by_time(_color_representations, _patch_center.t, current_time, _empty_pixmap);
		show_patch_pixbuf(_patch_slice, _patch_scale);
	}
}


void Patch_Weight_Rig::set_color_weight()
{
	_color_weight = _ui.color_weight_slider->get_value();

	Sequence* sequence = _hull->request_sequence();
	int current_time = _hull->request_current_time();

	if (sequence) {
		Shape patch_size = Shape(_patch_size, _patch_size, _patch_duration);
		_distances = calculate_distances(*sequence,_patch_center, patch_size, 0, _distance_mode, _distance_weight, _color_weight);
		_color_representations = draw_distances_with_color(*_distances, _gamma);
		_patch_slice = get_distance_representaton_by_time(_color_representations, _patch_center.t, current_time, _empty_pixmap);
		show_patch_pixbuf(_patch_slice, _patch_scale);
	}
}


void Patch_Weight_Rig::set_gamma()
{
	_gamma = _ui.gamma_slider->get_value();

	int current_time = _hull->request_current_time();

	if (_distances) {
		_color_representations = draw_distances_with_color(*_distances, _gamma);
		_patch_slice = get_distance_representaton_by_time(_color_representations, _patch_center.t, current_time, _empty_pixmap);
		show_patch_pixbuf(_patch_slice, _patch_scale);
	}
}

/* Private */

Glib::RefPtr<Gdk::Pixbuf> Patch_Weight_Rig::create_empty_pixbuf(int width, int height)
{
	Glib::RefPtr<Gdk::Pixbuf> empty_image = Gdk::Pixbuf::create(Gdk::COLORSPACE_RGB, false, 8, width, height);
	empty_image->fill(0);
	return empty_image;
}


Sequence* Patch_Weight_Rig::calculate_distances( Sequence &sequence,
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

	vector<OpticalFlowContainer*> forward_optical_flow_list = _hull->request_forward_optical_flow();
	vector<OpticalFlowContainer*> backward_optical_flow_list = _hull->request_backward_optical_flow();

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
				distances = marching.CalculateDistance(sequence, forward_optical_flow_list, backward_optical_flow_list, lookup_limits, patch_size, patch_center, AbstractMarchingAlgorithm::patch_shift_mode_none);
			} else if (_motion_compensation_mode == patch_shift_central) {
				distances = marching.CalculateDistance(sequence, forward_optical_flow_list, backward_optical_flow_list, lookup_limits, patch_size, patch_center, AbstractMarchingAlgorithm::patch_shift_mode_central);
			} else if (_motion_compensation_mode == patch_shift_weighted) {
				distances = marching.CalculateDistance(sequence, forward_optical_flow_list, backward_optical_flow_list, lookup_limits, patch_size, patch_center, AbstractMarchingAlgorithm::patch_shift_mode_weighted);
			} else {
				distances = marching.CalculateDistance(sequence, lookup_limits, patch_size, patch_center);
			}
			break;
		}
		case patch_space: {
			// TODO: maybe get patch first
			if (_motion_compensation_mode == pixelwise) {
				distances = marching.CalculateDistance(sequence, forward_optical_flow_list, backward_optical_flow_list, patch_size, patch_size, patch_center, AbstractMarchingAlgorithm::patch_shift_mode_none);
			} else if (_motion_compensation_mode == patch_shift_central) {
				distances = marching.CalculateDistance(sequence, forward_optical_flow_list, backward_optical_flow_list, patch_size, patch_size, patch_center, AbstractMarchingAlgorithm::patch_shift_mode_central);
			} else if (_motion_compensation_mode == patch_shift_weighted) {
				distances = marching.CalculateDistance(sequence, forward_optical_flow_list, backward_optical_flow_list, patch_size, patch_size, patch_center, AbstractMarchingAlgorithm::patch_shift_mode_weighted);
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

	// Update layer.
	Patch_Position_Layer *layer = find_or_create_patch_position_layer(_layer_manager);
	layer->clear_slice_origins();
	for (int i = 0;i < distances->GetTSize(); i++) {
		layer->set_slice_origin( distances->GetFrame(i)->get_coordinates() );
	}
	//layer->set_current_time(_current_time);


	return distances;
}


Patch_Position_Layer* Patch_Weight_Rig::find_or_create_patch_position_layer(Layer_Manager layer_manager)
{
	const string key = "patch_position";
	Patch_Position_Layer *layer = dynamic_cast<Patch_Position_Layer* >(layer_manager.find_layer(key));
	if (!layer) {
		layer = new Patch_Position_Layer(key, "Patch Position");
		//layer->set_visibitity(_layers_visibility); //?????
		layer_manager.add_layer(layer);
	}

	return layer;
}


vector<Glib::RefPtr<Gdk::Pixbuf> > Patch_Weight_Rig::draw_distances_with_color(Sequence &distances, float gamma)
{
	DijkstraAlgorithm marching = DijkstraAlgorithm(0);

	Sequence *distanceRepresentation =
				marching.BuildColorDistanceRepresentation(distances, gamma);

	int patch_duration = distances.GetTSize();
	vector<Glib::RefPtr<Gdk::Pixbuf> > color_representations = vector<Glib::RefPtr<Gdk::Pixbuf> >(patch_duration);
	for (int i=0; i<patch_duration; i++) {
		color_representations[i] = wrap_raw_image_data(distanceRepresentation->GetFrame(i));
	}

	delete distanceRepresentation;

	return color_representations;
}


/**
 * Converts Image into Gdk::Pixbuf
 */
Glib::RefPtr<Gdk::Pixbuf> Patch_Weight_Rig::wrap_raw_image_data(Image *image)
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


/*
 * From colored representation selects one for corresponding time point
 */
Glib::RefPtr<Gdk::Pixbuf> Patch_Weight_Rig::get_distance_representaton_by_time(
											vector<Glib::RefPtr<Gdk::Pixbuf> > representation,
											int patch_time_center,
											int current_time,
											Glib::RefPtr<Gdk::Pixbuf> empty_pixbuf)
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


void Patch_Weight_Rig::show_patch_pixbuf(Glib::RefPtr<Gdk::Pixbuf> pixbuf, int scale)
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


void Patch_Weight_Rig::update_coordinates()
{
	Glib::ustring coordinates = Glib::ustring::compose("x: %1; y: %2; t: %3", _patch_center.x, _patch_center.y, _patch_center.t);
	_ui.coordinates_label->set_label(coordinates);
}

