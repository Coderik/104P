/*
 * mask_module.cpp
 *
 *  Created on: Feb 4, 2014
 *      Author: Vadim Fedorov
 */

#include "mask_module.h"

void MaskModule::initialize(IModulable *modulable)
{
	_modulable = modulable;

	_modulable->signal_sequence_changed().connect( sigc::mem_fun(*this, &MaskModule::sequence_changed) );

	// create menu item for the module and assign it to the main menu
	Gtk::Menu *menu = Gtk::manage(new Gtk::Menu());
	Gtk::MenuItem *open_mask_menu_item = Gtk::manage(new Gtk::MenuItem("Open Mask..."));
	open_mask_menu_item->signal_activate().connect( sigc::mem_fun(*this, &MaskModule::open_mask) );
	menu->append(*open_mask_menu_item);

	_modulable->assign_menu(menu, "Mask");
}


SequenceMask* MaskModule::request_mask()
{
	// TODO: implement
	return 0;
}


/* Private */

void MaskModule::open_mask()
{
	// TODO: add filters
	string filename = _modulable->request_open_filename("Open Mask File");

	if (!filename.empty()) {
		// TODO: read mask file

		// TODO: ensure mask size matches current sequence

		// notify all who may concern about mask changing
		Request<IRig, IMaskAware> request;
		_modulable->request_active_rig(request);
		IMaskAware *mask_aware_rig = request.get_responce();
		if (mask_aware_rig) {
			mask_aware_rig->mask_changed();
		}
	}
}


void MaskModule::sequence_changed()
{
	// TODO: ensure mask size matches newly opened data
}




