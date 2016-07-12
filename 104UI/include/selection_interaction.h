/**
 * Copyright (C) 2016, Vadim Fedorov <coderiks@gmail.com>
 *
 * This program is free software: you can use, modify and/or
 * redistribute it under the terms of the simplified BSD
 * License. You should have received a copy of this license along
 * this program. If not, see
 * <http://www.opensource.org/licenses/bsd-license.html>.
 */

/// Created on: Mar 13, 2014

#ifndef SELECTION_INTERACTION_H_
#define SELECTION_INTERACTION_H_

#include "interaction.h"
#include "mask_group.h"

class SelectionInteraction : public Interaction
{
public:
	SelectionInteraction(MaskGroup *group);
	virtual ~SelectionInteraction() {}

	MaskSequenceFx get_mask();
	MaskGroup* get_group();
	void set_group(MaskGroup *group);

	// ?TODO: not sure if this signal is helpful or it just introduces more confusion
	typedef sigc::signal<void, MaskSequenceFx > type_signal_mask_changed;
	type_signal_mask_changed signal_mask_changed() {
		return _signal_mask_changed;
	}

protected:
	MaskGroup *_group;

private:
	type_signal_mask_changed _signal_mask_changed;
	sigc::connection _connection_mask_group_signal_mask_changed;

	void mask_changed(MaskGroup *group);
};



#endif /* SELECTION_INTERACTION_H_ */
