/*
 * a_parameters_set.h
 *
 *  Created on: Sep 09, 2013
 *      Author: Vadim Fedorov
 */

#ifndef A_PARAMETERS_SET_H_
#define A_PARAMETERS_SET_H_

#include <gtkmm/box.h>
#include <sigc++/sigc++.h>

class AParametersSet
{
public:
	virtual ~AParametersSet() {}

	virtual Gtk::VBox* get_ui() = 0;

	typedef sigc::signal<void> type_signal_changed;
	type_signal_changed signal_changed()
	{
		return _signal_changed;
	}

protected:
	type_signal_changed _signal_changed;
};


#endif /* A_PARAMETERS_SET_H_ */
