/*
 * interaction.h
 *
 *  Created on: Sep 30, 2013
 *      Author: Vadim Fedorov
 */

#ifndef INTERACTION_H_
#define INTERACTION_H_

#include <string>
#include <gdk/gdk.h>
#include "mouse_event.h"

using namespace std;

class Interaction
{
public:
	virtual ~Interaction() {}

	virtual void left_button_pressed(MouseEvent event) {};
	virtual void left_button_drag(MouseEvent event) {};
	virtual void left_button_released(MouseEvent event) {};
	virtual void key_pressed(GdkEventKey* event) {};

	string get_display_name()
	{
		return _display_name;
	}

protected:
	string _display_name;

};


#endif /* INTERACTION_H_ */
