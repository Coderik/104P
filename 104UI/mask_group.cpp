/**
 * Copyright (C) 2016, Vadim Fedorov <coderiks@gmail.com>
 *
 * This program is free software: you can use, modify and/or
 * redistribute it under the terms of the simplified BSD
 * License. You should have received a copy of this license along
 * this program. If not, see
 * <http://www.opensource.org/licenses/bsd-license.html>.
 */

/// Created on: Mar 11, 2014

#include "mask_group.h"

MaskGroup::MaskGroup()
{
}


MaskGroup::~MaskGroup()
{
}


MaskSequenceFx MaskGroup::get_mask()
{
	return _mask;
}


void MaskGroup::set_mask(const MaskSequenceFx &mask)
{
	_mask = mask;

	_signal_mask_changed.emit(this);
}


void MaskGroup::add_mask(const MaskSequenceFx &mask)
{
	if (_mask.is_empty()) {
		set_mask(mask);
		return;
	}

	if (_mask.size() != mask.size()) {
		return;
	}

	Mask::iterator it;
	for (uint t = 0; t < mask.size_t(); t++) {
		Mask mask_frame = _mask.frame(t);
		for (it = mask[t].begin(); it != mask[t].end(); ++it) {
			mask_frame.mask(*it);
		}
	}

	_signal_mask_changed.emit(this);	// TODO: emit only if mask has changed
}


void MaskGroup::subtract_mask(const MaskSequenceFx &mask)
{
	if (_mask.is_empty() || _mask.size() != mask.size()) {
		return;
	}

	Mask::iterator it;
	for (uint t = 0; t < mask.size_t(); t++) {
		Mask mask_frame = _mask.frame(t);
		for (it = mask[t].begin(); it != mask[t].end(); ++it) {
			mask_frame.unmask(*it);
		}
	}

	_signal_mask_changed.emit(this);	// TODO: emit only if mask has changed
}


void MaskGroup::intersect_mask(const MaskSequenceFx &mask)
{
	if (_mask.is_empty() || _mask.size() != mask.size()) {
		return;
	}

	bool has_changed = false;
	Mask::iterator it;
	for (uint t = 0; t < mask.size_t(); t++) {
		Mask this_mask_frame = _mask.frame(t);
		Mask other_mask_frame = mask.frame(t);
		for (it = _mask[t].begin(); it != _mask[t].end(); ++it) {
			if (!other_mask_frame(*it)) {
				this_mask_frame.unmask(*it);
				has_changed = true;
			}
		}
	}

	if (has_changed) {
		_signal_mask_changed.emit(this);
	}
}


bool MaskGroup::is_empty()
{
	if (_mask.is_empty()) {
		return true;
	}

	for (uint t = 0; t < _mask.size_t(); t++) {
		if (_mask[t].begin() != _mask[t].end()) {
			return false;
		}
	}

	return true;
}
