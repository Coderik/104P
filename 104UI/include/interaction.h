/**
 * Copyright (C) 2016, Vadim Fedorov <coderiks@gmail.com>
 *
 * This program is free software: you can use, modify and/or
 * redistribute it under the terms of the simplified BSD
 * License. You should have received a copy of this license along
 * this program. If not, see
 * <http://www.opensource.org/licenses/bsd-license.html>.
 */

/// Created on: Sep 30, 2013

#ifndef INTERACTION_H_
#define INTERACTION_H_

#include <string>
#include <gdk/gdk.h>
#include "mouse_event.h"

class Interaction
{
public:
	virtual ~Interaction() {}

	virtual void left_button_pressed(MouseEvent event) {};
	virtual void left_button_drag(MouseEvent event) {};
	virtual void left_button_released(MouseEvent event) {};
	virtual void key_pressed(GdkEventKey* event) {};

	std::string get_display_name()
	{
		return _display_name;
	}

	std::string get_tooltip_text()
	{
		return _tooltip_text;
	}

	std::string get_icon_name()
	{
		return _icon_name;
	}

protected:
	std::string _display_name;
	std::string _tooltip_text;
	std::string _icon_name;

};


#endif /* INTERACTION_H_ */
