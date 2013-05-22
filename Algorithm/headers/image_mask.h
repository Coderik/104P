/*
 * image_mask.h
 *
 *  Created on: Apr 20, 2013
 *      Author: Vadim Fedorov
 */

#ifndef IMAGE_MASK_H_
#define IMAGE_MASK_H_

#include "image.h"
#include "i_iterable_mask.h"
#include "mask_iterator.h"

// TODO: may be Sequence_Mask would be enough?
class ImageMask : public Image<bool>, public IIterableMask
{
public:
	typedef MaskIterator iterator;

	ImageMask();
	ImageMask(int x_size,int y_size);
	ImageMask(int x_size,int y_size, bool value);
	ImageMask(ImageMask &source);
	~ImageMask();

	iterator begin();
    iterator end();
    iterator rbegin();
    iterator rend();

    void mask(int x, int y);
    void unmask(int x, int y);
    void invert();

	/* I_Iterable_Mask methods */
	virtual Point first();
	virtual Point last();
	virtual Point next(const Point current);
	virtual Point prev(const Point current);
};

#endif /* IMAGE_MASK_H_ */