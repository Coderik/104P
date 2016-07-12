/**
 * Copyright (C) 2016, Vadim Fedorov <coderiks@gmail.com>
 *
 * This program is free software: you can use, modify and/or
 * redistribute it under the terms of the simplified BSD
 * License. You should have received a copy of this license along
 * this program. If not, see
 * <http://www.opensource.org/licenses/bsd-license.html>.
 */

/// Created on: Sep 29, 2013

#include "point_picker.h"

PointPicker::PointPicker()
{
	_display_name = "Point";
	_tooltip_text = "Select a point";
	_icon_name = "gdpg_point_selection";	// TODO: retrieve app name somehow to use it as a prefix

	Glib::RefPtr<Gtk::IconTheme> theme = Gtk::IconTheme::get_default();
	if (!theme->has_icon(_icon_name)) {
		theme->add_builtin_icon(_icon_name, 24, Gdk::Pixbuf::create_from_inline(sizeof(icon_point_selection), icon_point_selection));
	}
}


void PointPicker::left_button_pressed(MouseEvent event)
{
	EventArgs args;

	args.point.x = event.x;
	args.point.y = event.y;
	args.point.t = event.t;
	args.is_absolute = true;
	args.is_shift_modifier = event.modifiers & MouseEvent::SHIFT_MODIFIER;
	args.is_control_modifier = event.modifiers & MouseEvent::CONTROL_MODIFIER;
	args.is_alt_modifier = event.modifiers & MouseEvent::ALT_MODIFIER;

	_signal_point_updated.emit(args);
}


void PointPicker::key_pressed(GdkEventKey* event)
{
	EventArgs args;
	switch(event->keyval) {
		case GDK_KEY_W :
		case GDK_KEY_w :
			args.point = Point(0, -1, 0);
			args.is_absolute = false;
			args.is_shift_modifier = event->state & MouseEvent::SHIFT_MODIFIER;
			args.is_control_modifier = event->state & MouseEvent::CONTROL_MODIFIER;
			args.is_alt_modifier = event->state & MouseEvent::ALT_MODIFIER;
			_signal_point_updated.emit(args);
			break;
		case GDK_KEY_A :
		case GDK_KEY_a :
			args.point = Point(-1, 0, 0);
			args.is_absolute = false;
			args.is_shift_modifier = event->state & MouseEvent::SHIFT_MODIFIER;
			args.is_control_modifier = event->state & MouseEvent::CONTROL_MODIFIER;
			args.is_alt_modifier = event->state & MouseEvent::ALT_MODIFIER;
			_signal_point_updated.emit(args);
			break;
		case GDK_KEY_S :
		case GDK_KEY_s :
			args.point = Point(0, 1, 0);
			args.is_absolute = false;
			args.is_shift_modifier = event->state & MouseEvent::SHIFT_MODIFIER;
			args.is_control_modifier = event->state & MouseEvent::CONTROL_MODIFIER;
			args.is_alt_modifier = event->state & MouseEvent::ALT_MODIFIER;
			_signal_point_updated.emit(args);
			break;
		case GDK_KEY_D :
		case GDK_KEY_d :
			args.point = Point(1, 0, 0);
			args.is_absolute = false;
			args.is_shift_modifier = event->state & MouseEvent::SHIFT_MODIFIER;
			args.is_control_modifier = event->state & MouseEvent::CONTROL_MODIFIER;
			args.is_alt_modifier = event->state & MouseEvent::ALT_MODIFIER;
			_signal_point_updated.emit(args);
			break;
		default:
			break;
	}
}
