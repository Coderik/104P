/*
 * mask_module.cpp
 *
 *  Created on: Feb 4, 2014
 *      Author: Vadim Fedorov
 */

#include "mask_module.h"


MaskModule::MaskModule()
{
	_mask = 0;
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

	_modulable->assign_menu(menu, "Mask");
}


SequenceMask* MaskModule::request_mask()
{
	// TODO: use some kind of smart pointer
	return new SequenceMask(*_mask);
}


Glib::RefPtr<Gdk::Pixbuf> MaskModule::provide_mask_view(unsigned int time)
{
	Glib::RefPtr<Gdk::Pixbuf> buffer;

	if (_mask) {
		// draw mask
		buffer = Gdk::Pixbuf::create(Gdk::COLORSPACE_RGB, false, 8, _mask->get_size_x(), _mask->get_size_y());

		int rowstride = buffer->get_rowstride();	// get internal row length
		int number_of_channels = buffer->get_n_channels();
		guint8* data = buffer->get_pixels();

		for (int y = 0; y < _mask->get_size_y(); y++) {
			for (int x = 0; x < _mask->get_size_x(); x++) {
				char color = (_mask->get_value(x, y, time)) ? 255 : 0;

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
	Sequence<float> *sequence = _modulable->request_sequence();

	if (!sequence) {
		return;
	}

	// TODO: add filters
	string filename = _modulable->request_open_filename("Open Mask File");

	if (!filename.empty()) {
		// read mask file
		Image<float> *mask_image = IOUtility::read_pgm_image(filename);

		// ensure mask size matches current sequence
		if (mask_image->get_size_x() != sequence->get_size_x() || mask_image->get_size_y() != sequence->get_size_y()) {
			// TODO: make some notification
			return;
		}

		// [re]allocate memory
		if (_mask) {
			delete _mask;
		}
		_mask = new SequenceMask(mask_image->get_size_x(), mask_image->get_size_y(), sequence->get_size_t());

		// create sequence mask from the mask image
		// TODO: add constructor to do this
		for (int x = 0; x < mask_image->get_size_x(); x++) {
			for (int y = 0; y < mask_image->get_size_y(); y++) {
				if (mask_image->get_value(x, y) > 0) {
					// duplicate mask to all frames
					for (int t = 0; t < sequence->get_size_t(); t++) {
						_mask->mask(x, y, t);
					}
				}
			}
		}

		delete mask_image;

		// add view
		if (_mask_view.is_empty()) {
			_mask_view = _modulable->add_view("Mask", sigc::mem_fun(*this, &MaskModule::provide_mask_view));
		}

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
	if (!_mask) {
		return;
	}

	// drop mask, if its size does not match newly opened data
	Sequence<float> *sequence = _modulable->request_sequence();
	if (!sequence || sequence->get_size() != _mask->get_size()) {
		delete _mask;
		_mask = 0;

		_modulable->remove_view(_mask_view);
		_mask_view = Descriptor::empty();
	}
}




