/*
 * i_modulable.h
 *
 *  Created on: Feb 4, 2014
 *      Author: Vadim Fedorov
 */

#ifndef I_MODULABLE_H_
#define I_MODULABLE_H_

#include <string>
#include <gtkmm/menu.h>
#include <gtkmm/filefilter.h>
#include <sigc++/sigc++.h>

#include "sequence.h"

using namespace std;

class IModulable
{
public:
	virtual ~IModulable() {}

	virtual sigc::signal<void> signal_sequence_changed() = 0;

	virtual void assign_menu(Gtk::Menu *menu, string title) = 0;

	virtual Sequence<float>* request_sequence() = 0;
	virtual string request_open_filename(string dialog_title, Glib::RefPtr<Gtk::FileFilter> filter = Glib::RefPtr<Gtk::FileFilter>()) = 0;
};





#endif /* I_MODULABLE_H_ */
