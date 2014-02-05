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
#include "request.h"
#include "i_rig.h"

using namespace std;

class IModulable
{
public:
	virtual ~IModulable() {}

	virtual sigc::signal<void> signal_sequence_changed() = 0;

	virtual void assign_menu(Gtk::Menu *menu, string title) = 0;

	virtual Sequence<float>* request_sequence() = 0;
	virtual string request_open_filename(string dialog_title, Glib::RefPtr<Gtk::FileFilter> filter = Glib::RefPtr<Gtk::FileFilter>()) = 0;

	// NOTE: This is one possible way to notify an active rig about something from a module. Rig should implement some interface which is known by a module.
	//       For example, mask providing module can request for a rig which implement IMaskAware interface with a single mask_changed() method.
	//       Module have to request for an active rig every time and not store it internally, because active rig might change.
	//TODO: Another way should be based on module's proxy and signals
	virtual void request_active_rig(RequestBase<IRig> &request) = 0;
};





#endif /* I_MODULABLE_H_ */
