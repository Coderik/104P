/*
 * point_picker.h
 *
 *  Created on: Sep 29, 2013
 *      Author: Vadim Fedorov
 */

#ifndef POINT_PICKER_H_
#define POINT_PICKER_H_

#include <gtkmm/icontheme.h>
#include <sigc++/sigc++.h>
#include "interaction.h"
#include "i_hull.h"
#include "point.h"
#include "mouse_event.h"
#include "../icons/icon_point_selection_24.h"

class PointPicker : public Interaction
{
public:
	struct EventArgs
	{
		Point point;
		bool is_absolute;
		bool is_shift_modifier;
		bool is_control_modifier;
		bool is_alt_modifier;
	};

	PointPicker();

	virtual void left_button_pressed(MouseEvent event);
	virtual void key_pressed(GdkEventKey* event);

	typedef sigc::signal<void, EventArgs> type_signal_point_updated;
	type_signal_point_updated signal_point_updated() {
		return _signal_point_updated;
	}

private:
	type_signal_point_updated _signal_point_updated;
};


#endif /* POINT_PICKER_H_ */
