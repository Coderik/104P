/*
 * image_mask.h
 *
 *  Created on: Apr 20, 2013
 *      Author: Vadim Fedorov
 */

#ifndef IMAGE_MASK_H_
#define IMAGE_MASK_H_

#include <vector>
#include <stdexcept>

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
	ImageMask(uint size_x, uint size_y);
	ImageMask(uint size_x, uint size_y, bool default_value);
	ImageMask(Shape size);
	ImageMask(Shape size, bool default_value);
	ImageMask(const Image<bool> &source);			// deep copy
	ImageMask(const ConstImage<bool> &source);		// deep copy
	ImageMask(const ImageMask &source);				// ref++
	//ImageMask(const ConstImageMask &source);		// deep copy
	~ImageMask();

	// TODO: add assignment operators

	iterator begin() const;
	iterator end() const;
	iterator rbegin() const;
	iterator rend() const;

	/*
	 * Returns a reference to the element without range checking. Invalidates internal cache.
	 */
	virtual bool& operator() (uint x, uint y);
	virtual bool& operator() (uint x, uint y, uint channel);
	virtual bool& operator() (Point p);
	virtual bool& operator() (Point p, uint channel);

	/*
	 * Returns a reference to the element with range checking (throws std::out_of_range exception, if out of range). Invalidates internal cache.
	 */
	virtual bool& at(uint x, uint y);
	virtual bool& at(uint x, uint y, uint channel);
	virtual bool& at(Point p);
	virtual bool& at(Point p, uint channel);

	/*
	 * Returns the element without range checking. Does not affect internal cache.
	 */
	// ?TODO: can be inlined? if not, probably do not need these, because if performance is important, the data will be accessed directly.
	bool get(uint x, uint y);
	bool get(uint x, uint y, uint channel);
	bool get(Point p);
	bool get(Point p, uint channel);

	/*
	 * Returns the element with range checking (returns 'false', if out of range). Does not affect internal cache.
	 */
	bool test(uint x, uint y);
	bool test(uint x, uint y, uint channel);
	bool test(Point p);
	bool test(Point p, uint channel);

	/*
	 * Sets element to 'true' with range checking (does nothing, if out of range). Invalidates internal cache.
	 */
	void mask(uint x, uint y);
	void mask(uint x, uint y, uint channel);
	void mask(Point p);
	void mask(Point p, uint channel);

	/*
	 * Sets element to 'false' with range checking (does nothing, if out of range). Invalidates internal cache.
	 */
	void unmask(uint x, uint y);
	void unmask(uint x, uint y, uint channel);
	void unmask(Point p);
	void unmask(Point p, uint channel);

    void invert();

    vector<Point> get_masked_points() const;

	/* I_Iterable_Mask methods */
    // TODO: handle channels, otherwise rewrite ctors to work with only one channel
	virtual Point first() const;
	virtual Point last() const;
	virtual Point next(const Point &current) const;
	virtual Point prev(const Point &current) const;

private:
	struct __Internal
	{
		Point first;
		Point last;
		bool is_first_last_valid;
		bool is_points_cache_valid;
		vector<Point> points_cache;
	};

	mutable __Internal *_internal;

	inline void init(Point first, Point last, bool is_first_last_valid);
	inline void actualize_first_last() const;
};

#endif /* IMAGE_MASK_H_ */
