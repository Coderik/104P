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

class SequenceMask : public Sequence<bool>, public IIterableMask
{
public:
	typedef MaskIterator iterator;

	SequenceMask(int x_size, int y_size, int t_size, bool value);
	SequenceMask(int x_size, int y_size, int t_size);
	SequenceMask(int x_size, int y_size);
	SequenceMask(Image<bool> *first_frame);
	SequenceMask(SequenceMask& source);
	~SequenceMask();

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
	virtual Point next(const Point current) const;
	virtual Point prev(const Point current) const;

};


#endif /* SEQUENCE_MASK_H_ */
