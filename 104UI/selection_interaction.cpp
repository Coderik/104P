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

#include "selection_interaction.h"

SelectionInteraction::SelectionInteraction(MaskGroup *group)
{
	_group = (group) ? group : new MaskGroup();

	_connection_mask_group_signal_mask_changed =
			_group->signal_mask_changed().connect( sigc::mem_fun(*this, &SelectionInteraction::mask_changed) );
}

MaskSequenceFx SelectionInteraction::get_mask()
{
	return _group->get_mask();
}


MaskGroup* SelectionInteraction::get_group()
{
	return _group;
}


void SelectionInteraction::set_group(MaskGroup *group)
{
	_connection_mask_group_signal_mask_changed.disconnect();
	_group = group;
	_connection_mask_group_signal_mask_changed =
			_group->signal_mask_changed().connect( sigc::mem_fun(*this, &SelectionInteraction::mask_changed) );
}

/* Private */

void SelectionInteraction::mask_changed(MaskGroup *group)
{
	// propagate event further
	_signal_mask_changed.emit(_group->get_mask());
}




