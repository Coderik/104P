/*
 * patch_weight_mode.cpp
 *
 *  Created on: Apr 9, 2013
 *      Author: Vadim Fedorov
 */

#include "patch_weight_mode.h"

Patch_Weight_Mode::Patch_Weight_Mode()
{
	_ui = Patch_Weight_UI_Container();
}


void Patch_Weight_Mode::initialize(Sequence *sequence)
{
	// TODO: check if ui is been built
	// TODO: check if sequence is the same
	_sequence = sequence;

	// Set initial coordinates
	_patch_center.x = _sequence->GetXSize() / 2;
	_patch_center.y = _sequence->GetYSize() / 2;
	_patch_center.t = 0;
	_patch_duration = 1;
	_current_time = 0;
	update_coordinates();

	// Get patch, then color representation of distances
	Shape patch_size = Shape(_patch_size, _patch_size, _patch_duration);
	_distances = calculate_distances(*_sequence,_patch_center, patch_size, 0, _distance_mode, _distance_weight, _color_weight);
	_color_representations = draw_distances_with_color(*_distances, _gamma);

	if (_sequence->GetTSize() > 1) {
		// Adjust UI for a video sequence
		_ui.patch_duration_picker->set_active(0);
		_ui.patch_duration_picker->set_sensitive(true);
		_ui.motion_compensation_picker->set_sensitive(false);

		// Get pixmap for current time point
		_patch_slice = GetDistanceRepresentatonByTime(_color_representations, _patch_center.t, _current_time, _empty_pixmap);

	} else {
		// Adjust UI for a stil image
		_ui.patch_duration_picker->set_active(0);
		_ui.patch_duration_picker->set_sensitive(false);

		// Get pixmap for the only time point.
		_patch_slice = _color_representations[0];
	}

	// Show image and patch
	ShowPatchPixbuf(_patch_slice, _patch_scale);
	update_image_control(_current_time);
}


/* Private */


void Patch_Weight_Mode::update_coordinates()
{
	Glib::ustring coordinates = Glib::ustring::compose("x: %1; y: %2; t: %3", _patch_center.x, _patch_center.y, _patch_center.t);
	_ui.coordinates_label->set_label(coordinates);
}


