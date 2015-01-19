/*
 * mask_module.cpp
 *
 *  Created on: Feb 4, 2014
 *      Author: Vadim Fedorov
 */

#include "mask_module.h"


MaskModule::MaskModule()
{
	_modulable = 0;
}


void MaskModule::initialize(IModulable *modulable)
{
	_modulable = modulable;

	_modulable->signal_sequence_changed().connect( sigc::mem_fun(*this, &MaskModule::sequence_changed) );

	// create menu item for the module and assign it to the main menu
	Gtk::Menu *menu = Gtk::manage(new Gtk::Menu());
	Gtk::MenuItem *open_mask_menu_item = Gtk::manage(new Gtk::MenuItem("Open Mask..."));
	open_mask_menu_item->signal_activate().connect( sigc::mem_fun(*this, &MaskModule::open_mask) );
	menu->append(*open_mask_menu_item);
	Gtk::MenuItem *save_mask_menu_item = Gtk::manage(new Gtk::MenuItem("Save Mask"));
	save_mask_menu_item->signal_activate().connect( sigc::mem_fun(*this, &MaskModule::save_mask) );
	menu->append(*save_mask_menu_item);
	Gtk::MenuItem *save_mask_as_menu_item = Gtk::manage(new Gtk::MenuItem("Save Mask As..."));
	save_mask_as_menu_item->signal_activate().connect( sigc::mem_fun(*this, &MaskModule::save_mask_as) );
	menu->append(*save_mask_as_menu_item);
	Gtk::MenuItem *separator_menu_item = Gtk::manage(new Gtk::SeparatorMenuItem());
	menu->append(*separator_menu_item);
	Gtk::MenuItem *set_mask_menu_item = Gtk::manage(new Gtk::MenuItem("Replace Mask"));
	set_mask_menu_item->signal_activate().connect( sigc::mem_fun(*this, &MaskModule::set_mask) );
	_signal_enable_mask_actions_menu_items.connect( sigc::mem_fun(*set_mask_menu_item, &Gtk::MenuItem::set_sensitive) );
	menu->append(*set_mask_menu_item);
	Gtk::MenuItem *add_mask_menu_item = Gtk::manage(new Gtk::MenuItem("Add to Mask"));
	add_mask_menu_item->signal_activate().connect( sigc::mem_fun(*this, &MaskModule::add_mask) );
	_signal_enable_mask_actions_menu_items.connect( sigc::mem_fun(*add_mask_menu_item, &Gtk::MenuItem::set_sensitive) );
	menu->append(*add_mask_menu_item);
	Gtk::MenuItem *subtract_mask_menu_item = Gtk::manage(new Gtk::MenuItem("Subtract from Mask"));
	subtract_mask_menu_item->signal_activate().connect( sigc::mem_fun(*this, &MaskModule::subtract_mask) );
	_signal_enable_mask_actions_menu_items.connect( sigc::mem_fun(*subtract_mask_menu_item, &Gtk::MenuItem::set_sensitive) );
	menu->append(*subtract_mask_menu_item);
	Gtk::MenuItem *intersect_mask_menu_item = Gtk::manage(new Gtk::MenuItem("Intersect with Mask"));
	intersect_mask_menu_item->signal_activate().connect( sigc::mem_fun(*this, &MaskModule::intersect_mask) );
	_signal_enable_mask_actions_menu_items.connect( sigc::mem_fun(*intersect_mask_menu_item, &Gtk::MenuItem::set_sensitive) );
	menu->append(*intersect_mask_menu_item);

	_modulable->assign_menu(menu, "Mask");

	// disable 'mask actions' menu items
	_signal_enable_mask_actions_menu_items.emit(false);
}


MaskSequenceFx MaskModule::request_mask()
{
	return _mask;
}


Glib::RefPtr<Gdk::Pixbuf> MaskModule::provide_mask_view(unsigned int time)
{
	Glib::RefPtr<Gdk::Pixbuf> buffer;

	if (_mask) {
		// draw mask
		buffer = Gdk::Pixbuf::create(Gdk::COLORSPACE_RGB, false, 8, _mask.size_x(), _mask.size_y());

		int rowstride = buffer->get_rowstride();	// get internal row length
		int number_of_channels = buffer->get_n_channels();
		guint8* data = buffer->get_pixels();

		for (uint y = 0; y < _mask.size_y(); y++) {
			for (uint x = 0; x < _mask.size_x(); x++) {
				char color = (_mask[time](x, y)) ? 255 : 0;

				for (int c = 0; c < number_of_channels; c++) {
					/// NOTE: 'rowstride' is the length of internal representation of a row
					/// inside the Pixbuf, and it could differ from x_size*number_of_channels.
					int index = y*rowstride + x*number_of_channels + c;
					data[index] = color;
				}
			}
		}
	}

	return buffer;
}


/* Private */

void MaskModule::open_mask()
{
	SequenceFx<float> sequence = _modulable->request_sequence();

	if (sequence.is_empty()) {
		return;
	}

	// TODO: add filters
	string file_name = _modulable->request_open_dialog_result("Open Mask File");

	if (!file_name.empty()) {
		// read mask file
		Image<float> mask_image = IOUtility::read_pgm_image(file_name);

		// ensure mask size matches current sequence
		if (mask_image.size_x() != sequence.size_x() || mask_image.size_y() != sequence.size_y()) {
			// TODO: make some notification
			return;
		}

		_mask_file_name = file_name;

		// [re]assign mask
		_mask = MaskSequence(mask_image.size_x(), mask_image.size_y(), sequence.size_t());

		// enable 'mask actions' menu items
		_signal_enable_mask_actions_menu_items.emit(true);

		// create sequence mask from the mask image
		// TODO: add constructor to do this
		for (uint t = 0; t < sequence.size_t(); t++) {
			Mask mask_frame = _mask.frame(t);
			for (uint x = 0; x < mask_image.size_x(); x++) {
				for (uint y = 0; y < mask_image.size_y(); y++) {
					if (mask_image(x, y) > 0) {
						// duplicate mask to all frames
						mask_frame.mask(x, y);
					}
				}
			}
		}

		// add view
		if (_mask_view.is_empty()) {
			_mask_view = _modulable->add_view("Mask", sigc::mem_fun(*this, &MaskModule::provide_mask_view));
		}

		// set mask immediately after loading in case if there were no mask before
		Request<IRig, IMaskAware> request;
		_modulable->request_active_rig(request);
		IMaskAware *mask_aware_rig = request.get_responce();
		if (mask_aware_rig) {
			MaskGroup *group = mask_aware_rig->get_mask_group();
			if (group && group->is_empty()) {
				group->set_mask(_mask);
			}
		}
	}
}


void MaskModule::save_mask()
{
	MaskSequenceFx mask;

	// request current mask from a rig
	Request<IRig, IMaskAware> request;
	_modulable->request_active_rig(request);
	IMaskAware *mask_aware_rig = request.get_responce();
	if (mask_aware_rig) {
		MaskGroup *group = mask_aware_rig->get_mask_group();
		if (group) {
			mask = group->get_mask();
		}
	}

	if (mask.is_empty()) {
		return;
	}

	if (_mask_file_name.empty()) {
		_mask_file_name = _modulable->request_save_dialog_result("Save Mask As");
	}
	string file_name = _mask_file_name;

	// set that mask as current
	_mask = mask;

	// enable 'mask actions' menu items
	_signal_enable_mask_actions_menu_items.emit(true);

	// write mask
	// TODO: deal also with 3D masks
	// TODO: add confirmation dialog
	IOUtility::write_pgm_image(file_name, Visualization::mask_to_greyscale(mask.frame(0)));

	// add view or request its redrawing
	if (_mask_view.is_empty()) {
		_mask_view = _modulable->add_view("Mask", sigc::mem_fun(*this, &MaskModule::provide_mask_view));
	} else {
		_modulable->queue_view_draw(_mask_view);
	}
}


void MaskModule::save_mask_as()
{
	MaskSequenceFx mask;

	Request<IRig, IMaskAware> request;
	_modulable->request_active_rig(request);
	IMaskAware *mask_aware_rig = request.get_responce();
	if (mask_aware_rig) {
		MaskGroup *group = mask_aware_rig->get_mask_group();
		if (group) {
			mask = group->get_mask();
		}
	}

	if (mask.is_empty()) {
		return;
	}

	// TODO: add filters
	string file_name = _modulable->request_save_dialog_result("Save Mask As");

	// write mask
	// TODO: deal also with 3D masks
	IOUtility::write_pgm_image(file_name, Visualization::mask_to_greyscale(mask.frame(0)));
}


void MaskModule::set_mask()
{
	if (_mask.is_empty()) {
		return;
	}

	Request<IRig, IMaskAware> request;
	_modulable->request_active_rig(request);
	IMaskAware *mask_aware_rig = request.get_responce();
	if (mask_aware_rig) {
		MaskGroup *group = mask_aware_rig->get_mask_group();
		if (group) {
			group->set_mask(_mask);
		}
	}
}


void MaskModule::add_mask()
{
	if (_mask.is_empty()) {
		return;
	}

	Request<IRig, IMaskAware> request;
	_modulable->request_active_rig(request);
	IMaskAware *mask_aware_rig = request.get_responce();
	if (mask_aware_rig) {
		MaskGroup *group = mask_aware_rig->get_mask_group();
		if (group) {
			group->add_mask(_mask);
		}
	}
}


void MaskModule::subtract_mask()
{
	if (_mask.is_empty()) {
		return;
	}

	Request<IRig, IMaskAware> request;
	_modulable->request_active_rig(request);
	IMaskAware *mask_aware_rig = request.get_responce();
	if (mask_aware_rig) {
		MaskGroup *group = mask_aware_rig->get_mask_group();
		if (group) {
			group->subtract_mask(_mask);
		}
	}
}


void MaskModule::intersect_mask()
{
	if (_mask.is_empty()) {
		return;
	}

	Request<IRig, IMaskAware> request;
	_modulable->request_active_rig(request);
	IMaskAware *mask_aware_rig = request.get_responce();
	if (mask_aware_rig) {
		MaskGroup *group = mask_aware_rig->get_mask_group();
		if (group) {
			group->intersect_mask(_mask);
		}
	}
}


void MaskModule::sequence_changed()
{
	if (_mask.is_empty()) {
		return;
	}

	// drop mask, if its size does not match newly opened data
	SequenceFx<float> sequence = _modulable->request_sequence();
	if (sequence.is_empty() || sequence.size() != _mask.size()) {
		_mask = MaskSequence();

		_modulable->remove_view(_mask_view);
		_mask_view = Descriptor::empty();
	}
}




