/**
 * Copyright (C) 2016, Vadim Fedorov <coderiks@gmail.com>
 *
 * This program is free software: you can use, modify and/or
 * redistribute it under the terms of the simplified BSD
 * License. You should have received a copy of this license along
 * this program. If not, see
 * <http://www.opensource.org/licenses/bsd-license.html>.
 */

/// Created on: Feb 4, 2014

#ifndef I_MODULABLE_H_
#define I_MODULABLE_H_

#include <string>
#include <gtkmm/menu.h>
#include <gtkmm/filefilter.h>
#include <sigc++/sigc++.h>

#include "sequence.h"
#include "request.h"
#include "i_rig.h"
#include "descriptor.h"

using namespace std;

class IModulable
{
public:
	virtual ~IModulable() {}

	virtual sigc::signal<void> signal_sequence_changed() = 0;

	virtual void assign_menu(Gtk::Menu *menu, string title) = 0;

	virtual SequenceFx<float> request_sequence() = 0;
	virtual string request_sequence_path() = 0;
	virtual string request_open_dialog_result(string dialog_title, bool folder = false, Glib::RefPtr<Gtk::FileFilter> filter = Glib::RefPtr<Gtk::FileFilter>()) = 0;
	virtual string request_save_dialog_result(string dialog_title, Glib::RefPtr<Gtk::FileFilter> filter = Glib::RefPtr<Gtk::FileFilter>()) = 0;

	// NOTE: This is one possible way to notify an active rig about something from a module. Rig should implement some interface which is known by a module.
	//       For example, mask providing module can request for a rig which implement IMaskAware interface with a single mask_changed() method.
	//       Module have to request for an active rig every time and not store it internally, because active rig might change.
	//TODO: Another way should be based on module's proxy and signals
	virtual void request_active_rig(RequestBase<IRig> &request) = 0;

	virtual Descriptor add_view(string title, sigc::slot1<Glib::RefPtr<Gdk::Pixbuf>, int> provider) = 0;
	virtual bool alter_view(Descriptor view_descriptor, string title, sigc::slot1<Glib::RefPtr<Gdk::Pixbuf>, int> provider) = 0;
	virtual bool remove_view(Descriptor view_descriptor) = 0;
	virtual bool queue_view_draw(Descriptor view_descriptor) = 0;

	// TODO: think about the better api for this (but note, that encapsulating message stuff in IBackgroundWorker is not an option)
	virtual Descriptor add_background_work_info(sigc::slot0<void> cancel_slot, string message = "") = 0;
	virtual bool alter_background_work_info(Descriptor descriptor, string message) = 0;
	virtual bool remove_background_work_info(Descriptor descriptor) = 0;
};





#endif /* I_MODULABLE_H_ */
