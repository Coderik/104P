/*
 * mask_group.cpp
 *
 *  Created on: Mar 11, 2014
 *      Author: Vadim Fedorov
 */

#include "mask_group.h"

MaskGroup::MaskGroup()
{
	_mask = 0;
}


MaskGroup::~MaskGroup()
{
	if (_mask) {
		delete _mask;
	}
}


SequenceMask* MaskGroup::get_mask()
{
	return (_mask) ? new SequenceMask(*_mask) : 0;
}


void MaskGroup::set_mask(const SequenceMask &mask)
{
	if (_mask) {
		delete _mask;
	}
	_mask = new SequenceMask(mask);

	_signal_mask_changed.emit(this);
}


void MaskGroup::add_mask(const SequenceMask &mask)
{
	if (!_mask) {
		set_mask(mask);
	}

	if (_mask->get_size() != mask.get_size()) {
		return;
	}

	SequenceMask::iterator it;
	for (it = mask.begin(); it != mask.end(); ++it) {
		_mask->mask(*it);
	}

	_signal_mask_changed.emit(this);	// TODO: emit only if mask has changed
}


void MaskGroup::subtract_mask(const SequenceMask &mask)
{
	if (!_mask || _mask->get_size() != mask.get_size()) {
		return;
	}

	SequenceMask::iterator it;
	for (it = mask.begin(); it != mask.end(); ++it) {
		_mask->unmask(*it);
	}

	_signal_mask_changed.emit(this);	// TODO: emit only if mask has changed
}


void MaskGroup::intersect_mask(const SequenceMask &mask)
{
	if (!_mask || _mask->get_size() != mask.get_size()) {
		return;
	}

	bool has_changed = false;
	SequenceMask::iterator it;
	for (it = _mask->begin(); it != _mask->end(); ++it) {
		if (!mask.get_value(*it)) {
			_mask->unmask(*it);
			has_changed = true;
		}
	}

	if (has_changed) {
		_signal_mask_changed.emit(this);
	}
}


bool MaskGroup::is_empty()
{
	return !_mask || _mask->begin() == _mask->end();
}
