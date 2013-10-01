/*
 * interaction_manager.h
 *
 *  Created on: Oct 1, 2013
 *      Author: Vadim Fedorov
 */

#ifndef INTERACTION_MANAGER_H_
#define INTERACTION_MANAGER_H_

#include <gtkmm/box.h>
#include <sigc++/sigc++.h>
#include "interaction.h"

class InteractionManager
{
public:
	InteractionManager();

	Gtk::Box* get_ui();
	Interaction* get_active();
	void add_interation(Interaction *interaction);

	typedef sigc::signal<void> type_signal_ui_updated;
	type_signal_ui_updated signal_ui_updated() {
		return _signal_ui_updated;
	}

private:
	Gtk::Box *_ui;
	Interaction *_active;
	type_signal_ui_updated _signal_ui_updated;
};


#endif /* INTERACTION_MANAGER_H_ */
