/*
 * mask_module.h
 *
 *  Created on: Feb 4, 2014
 *      Author: Vadim Fedorov
 */

#ifndef MASK_MODULE_H_
#define MASK_MODULE_H_

#include <gtkmm/menuitem.h>
#include <gtkmm/filechooserdialog.h>

#include "i_module.h"
#include "i_mask_provider.h"

class MaskModule: public IModule, public IMaskProvider
{
public:
	virtual ~MaskModule() {}

	// IModule members
	virtual void initialize(IModulable *modulable);

	// IMaskProvider members
	virtual SequenceMask* request_mask();

private:
	IModulable *_modulable;

	// slots
	void open_mask();
	void sequence_changed();
};





#endif /* MASK_MODULE_H_ */