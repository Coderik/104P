/*
 * mask_group.h
 *
 *  Created on: Mar 11, 2014
 *      Author: Vadim Fedorov
 */

#ifndef MASK_GROUP_H_
#define MASK_GROUP_H_

#include <sigc++/sigc++.h>
#include "mask_sequence.h"

// TODO: when will become brave enough for switching to c++11, use shared_ptr for this class. Without doing this there is a possibility of memory leak.

class MaskGroup
{
public:
	MaskGroup();
	~MaskGroup();

	// returns a copy of the internal mask
	MaskSequenceFx get_mask();
	void set_mask(const MaskSequenceFx &mask);
	void add_mask(const MaskSequenceFx &mask);
	void subtract_mask(const MaskSequenceFx &mask);
	void intersect_mask(const MaskSequenceFx &mask);
	bool is_empty();

	typedef sigc::signal<void, MaskGroup*> type_signal_mask_changed;
	type_signal_mask_changed signal_mask_changed() {
		return _signal_mask_changed;
	}

private:
	type_signal_mask_changed _signal_mask_changed;
	MaskSequence _mask;
};



#endif /* MASK_GROUP_H_ */
