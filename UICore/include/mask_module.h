/*
 * mask_module.h
 *
 *  Created on: Feb 4, 2014
 *      Author: Vadim Fedorov
 */

#ifndef MASK_MODULE_H_
#define MASK_MODULE_H_

#include <gtkmm/menuitem.h>
#include <gtkmm/separatormenuitem.h>
#include <gtkmm/filechooserdialog.h>

#include "i_module.h"
#include "i_mask_provider.h"
#include "io_utility.h"

class MaskModule: public IModule, public IMaskProvider
{
public:
	MaskModule();
	virtual ~MaskModule() {}

	// IModule members
	virtual void initialize(IModulable *modulable);

	// IMaskProvider members
	virtual SequenceMask* request_mask();

	Glib::RefPtr<Gdk::Pixbuf> provide_mask_view(unsigned int time);

private:
	IModulable *_modulable;
	SequenceMask *_mask;
	Descriptor _mask_view;
	string _mask_file_name;

	// slots
	void open_mask();
	void save_mask();
	void save_mask_as();
	void set_mask();
	void add_mask();
	void subtract_mask();
	void intersect_mask();
	void sequence_changed();
};





#endif /* MASK_MODULE_H_ */
