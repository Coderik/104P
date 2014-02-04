/*
 * interaction_manager.h
 *
 *  Created on: Oct 1, 2013
 *      Author: Vadim Fedorov
 */

#ifndef INTERACTION_MANAGER_H_
#define INTERACTION_MANAGER_H_

#include <vector>
#include <string>
#include <gtkmm/box.h>
#include <gtkmm/toolbar.h>
#include <gtkmm/radiotoolbutton.h>
#include <gtkmm/radiobuttongroup.h>
#include <gtkmm/frame.h>
#include <gtkmm/stock.h>
#include <sigc++/sigc++.h>
#include "interaction.h"

using namespace std;

class InteractionManager
{
public:
	InteractionManager();

	Gtk::Box* get_ui();
	Interaction* get_active();
	void add_interation(Interaction *interaction);
	int get_interactions_amount();

	typedef sigc::signal<void> type_signal_ui_updated;
	type_signal_ui_updated signal_ui_updated() {
		return _signal_ui_updated;
	}

private:
	static const string DEFAULT_ICON_NAME;
	Gtk::Box *_ui;
	Interaction *_active;
	vector<Interaction* > _interactions;
	type_signal_ui_updated _signal_ui_updated;

	Gtk::Box* build_ui();
	void destroy_ui(Gtk::Box* ui);
	void set_active(Interaction *interaction);
};


#endif /* INTERACTION_MANAGER_H_ */
