/*
 * sequence_mask.h
 *
 *  Created on: Apr 20, 2013
 *      Author: Vadim Fedorov
 */

#ifndef SEQUENCE_MASK_H_
#define SEQUENCE_MASK_H_

#include "sequence.h"
#include "i_iterable_mask.h"
#include "mask_iterator.h"
#include "image_mask.h"

class SequenceMask : public Sequence<bool>, public IIterableMask
{
public:
	typedef MaskIterator iterator;

	SequenceMask(int size_x, int size_y, int size_t, bool value);
	SequenceMask(int size_x, int size_y, int size_t);
	SequenceMask(int size_x, int size_y);
	SequenceMask(Shape size);
	SequenceMask(Image<bool> *first_frame);
	SequenceMask(SequenceMask& source);
	~SequenceMask();

	ImageMask* get_mask_frame(int t) const;

	iterator begin() const;
	iterator end() const;
	iterator rbegin() const;
	iterator rend() const;

	void mask(int x, int y, int t);
	void unmask(int x, int y, int t);
	void invert();

	/* I_Iterable_Mask methods */
	virtual Point first() const;
	virtual Point last() const;
	virtual Point next(const Point &current) const;
	virtual Point prev(const Point &current) const;

	static iterator empty_iterator();
};


#endif /* SEQUENCE_MASK_H_ */
