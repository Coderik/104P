/*
 * image_mask.h
 *
 *  Created on: Apr 20, 2013
 *      Author: Vadim Fedorov
 */

#ifndef IMAGE_MASK_H_
#define IMAGE_MASK_H_

#include <vector>

#include "image.h"
#include "i_iterable_mask.h"
#include "mask_iterator.h"

using namespace std;

// TODO: may be Sequence_Mask would be enough?
class ImageMask : public Image<bool>, public IIterableMask
{
public:
	typedef MaskIterator iterator;

	ImageMask();
	ImageMask(int size_x,int size_y);
	ImageMask(int size_x,int size_y, bool value);
	ImageMask(const Image<bool> &source);
	ImageMask(const ImageMask &source);
	~ImageMask();

	iterator begin() const;
	iterator end() const;
	iterator rbegin() const;
	iterator rend() const;

	virtual void set_value(int x, int y, bool value);
	virtual void set_value(Point p, bool value);

    void mask(int x, int y);
    void mask(Point p);
    void unmask(int x, int y);
    void unmask(Point p);
    void invert();

    vector<Point> get_masked_points() const;

	/* I_Iterable_Mask methods */
	virtual Point first() const;
	virtual Point last() const;
	virtual Point next(const Point &current) const;
	virtual Point prev(const Point &current) const;

private:
	mutable Point _first;
	mutable Point _last;
	mutable bool _is_first_last_valid;
	mutable bool _is_points_cache_valid;
	mutable vector<Point> _points_cache;

	inline void actualize_first_last() const;
};

#endif /* IMAGE_MASK_H_ */
