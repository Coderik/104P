/*
 * rectangular_selection.cpp
 *
 *  Created on: Sep 29, 2013
 *      Author: Vadim Fedorov
 */

#include "rectangular_selection.h"

RectangularSelection::RectangularSelection(IHull *hull, MaskGroup *group)
 : SelectionInteraction(group)
{
	_hull = hull;
	_display_name = "Rect Mask";
	_tooltip_text = "Select rectangular mask";
	_icon_name = "gdpg_rectangular_selection";	// TODO: retrieve app name somehow to use it as a prefix

	Glib::RefPtr<Gtk::IconTheme> theme = Gtk::IconTheme::get_default();
	if (!theme->has_icon(_icon_name)) {
		theme->add_builtin_icon(_icon_name, 24, Gdk::Pixbuf::create_from_inline(sizeof(icon_rectangular_selection), icon_rectangular_selection));
	}
}


void RectangularSelection::left_button_pressed(MouseEvent event)
{
	_begin.x = event.x;
	_begin.y = event.y;

	// TODO: it might be useful to store also t in order to select not only 2D rectangles, but also 3D 'cubes'
}


void RectangularSelection::left_button_drag(MouseEvent event)
{
	_end.x = event.x;
	_end.y = event.y;

	LayerManager *layer_manager = _hull->request_layer_manager();
	if (layer_manager) {
		RectangularSelectionLayer *layer =
			layer_manager->find_or_create_layer<RectangularSelectionLayer>("selection", "Selection");	// TODO: use some fancy name to avoid collisions

		int x = std::min(_begin.x, _end.x);
		int y = std::min(_begin.y, _end.y);
		int width = std::max(_begin.x, _end.x) - x;
		int height = std::max(_begin.y, _end.y) - y;
		int time = _hull->request_current_time();

		layer->set_data(x, y, width, height, time);
		layer->set_current_time(time);
	}
}


void RectangularSelection::left_button_released(MouseEvent event)
{
	if (_begin.x > 0 && _begin.y > 0) {
		_end.x = event.x;
		_end.y = event.y;

		SequenceFx<float> sequence = _hull->request_sequence();

		if (sequence) {
			MaskSequence mask = MaskSequence(sequence.size());

			int time = _hull->request_current_time();
			Mask mask_frame = mask.frame(time);
			for (int y = std::min(_begin.y, _end.y); y < std::max(_begin.y, _end.y); y++) {
				for (int x = std::min(_begin.x, _end.x); x < std::max(_begin.x, _end.x); x++) {
					mask_frame.mask(x, y);
				}
			}

			bool shift = event.modifiers & MouseEvent::SHIFT_MODIFIER;
			bool ctrl = event.modifiers & MouseEvent::CONTROL_MODIFIER;
			if ( shift && ctrl ) {
				_group->intersect_mask(mask);
			} else if ( shift ) {
				_group->add_mask(mask);
			} else if ( ctrl ) {
				_group->subtract_mask(mask);
			} else {	// neither shift, nor control
				_group->set_mask(mask);
			}
		}

		LayerManager *layer_manager = _hull->request_layer_manager();
		if (layer_manager) {
			// Clear selection
			RectangularSelectionLayer *layer =
				layer_manager->find_or_create_layer<RectangularSelectionLayer>("selection", "Selection");
			layer->set_data(0,0,0,0,0);
		}
	}
}
