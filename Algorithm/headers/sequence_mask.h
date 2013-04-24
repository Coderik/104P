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

class Sequence_Mask : public Sequence<bool>, public I_Iterable_Mask
{
public:
	typedef Mask_Iterator iterator;

	Sequence_Mask(int x_size, int y_size, int t_size, bool value);
	Sequence_Mask(int x_size, int y_size, int t_size);
	Sequence_Mask(int x_size, int y_size);
	Sequence_Mask(Image<bool> *first_frame);
	Sequence_Mask(Sequence_Mask& source);
	~Sequence_Mask();

	iterator begin();
    iterator end();
    iterator rbegin();
    iterator rend();

    void mask(int x, int y, int t);
    void unmask(int x, int y, int t);
    void invert();

	/* I_Iterable_Mask methods */
	virtual Point first();
	virtual Point last();
	virtual Point next(const Point current);
	virtual Point prev(const Point current);

};


#endif /* SEQUENCE_MASK_H_ */
