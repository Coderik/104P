/*
 * mask.h
 *
 *  Created on: Apr 20, 2013
 *      Author: Vadim Fedorov
 */

#ifndef MASK_H_
#define MASK_H_

#include <vector>
#include <stdexcept>

#include "image.h"
#include "mask_iterator.h"
#include "i_iterable_mask.h"

using namespace std;

/// forward declaration
class Mask;
class MaskIterator;

/**
 * Container for a 2d binary mask based on the FixedImage<bool> class.
 * Provides capabilities for iterating through the masked points and
 * retrieving masked points as a vector.
 *
 * @note By design class provides no capabilities for changing its data,
 * therefore, 'Fixed' here should be considered as 'Immutable'.
 */
class MaskFx : public ImageFx<bool>, public IIterableMask
{
friend class Mask;
public:
	typedef MaskIterator iterator;

	static iterator empty_iterator();

	MaskFx();
	MaskFx(uint size_x, uint size_y);
	MaskFx(uint size_x, uint size_y, bool default_value);
	MaskFx(Shape size);
	MaskFx(Shape size, bool default_value);
	MaskFx(const Image<bool> &source);			// deep copy
	MaskFx(const ImageFx<bool> &source);		// deep copy
	MaskFx(const Mask &source);					// without data copying, ref++
	MaskFx(const MaskFx &source);				// without data copying, ref++
	virtual ~MaskFx();

	MaskFx& operator= (const Mask &other);			// without data copying, ref++
	MaskFx& operator= (const MaskFx &other);		// without data copying, ref++

	iterator begin() const;
	iterator end() const;
	iterator rbegin() const;
	iterator rend() const;

	/// Returns the element without range checking. Does not affect internal cache.
	bool get(uint x, uint y) const;
	bool get(uint x, uint y, uint channel) const;
	bool get(Point p)  const;
	bool get(Point p, uint channel)  const;

	/// Returns the element with range checking (returns 'false', if out of range). Does not affect internal cache.
	bool test(uint x, uint y)  const;
	bool test(uint x, uint y, uint channel)  const;
	bool test(Point p)  const;
	bool test(Point p, uint channel)  const;

	/// Invokes deep copy.
	MaskFx clone() const;

	/// Invokes deep copy and then invert the cloned mask.
	MaskFx clone_invert() const;

	/// Returns masked points as a vector.
    vector<Point> masked_points() const;

	/// Methods used by MaskIterator
	virtual Point first() const;
	virtual Point last() const;
	virtual Point next(const Point &current) const;
	virtual Point prev(const Point &current) const;

protected:
	struct __Internal
	{
		Point first;
		Point last;
		bool is_first_last_valid;
		bool is_points_cache_valid;
		vector<Point> points_cache;
	};

	mutable __Internal *_internal;

	virtual void destroy() const;

	inline void init_internal(Point first, Point last, bool is_first_last_valid);
	inline void actualize_first_last() const;
};


/**
 * Container for a 2d binary mask based on the FixedImage<bool> class.
 * Provides capabilities for iterating through the masked points and
 * retrieving masked points as a vector.
 *
 * @note Extends the FixedMask class with the data modification capabilities.
 */
class Mask : public MaskFx
{
friend class MaskFx;
public:
	typedef MaskIterator iterator;

	Mask();
	Mask(uint size_x, uint size_y);
	Mask(uint size_x, uint size_y, bool default_value);
	Mask(Shape size);
	Mask(Shape size, bool default_value);
	Mask(const Image<bool> &source);			// deep copy
	Mask(const ImageFx<bool> &source);		// deep copy
	Mask(const Mask &source);					// without data copying, ref++
	Mask(const MaskFx &source);				// deep copy
	virtual ~Mask();

	Mask& operator= (const Mask &other);		// without data copying, ref++
	Mask& operator= (const MaskFx &other);	// deep copy

	// prevent hiding of const versions of these methods
	using ImageFx<bool>::operator();
	using ImageFx<bool>::at;

	/// Returns a reference to the element without range checking. Invalidates internal cache.
	bool& operator() (uint x, uint y);
	bool& operator() (uint x, uint y, uint channel);
	bool& operator() (Point p);
	bool& operator() (Point p, uint channel);

	/// Returns a reference to the element with range checking.
	/// Throws std::out_of_range exception, if out of range. Invalidates internal cache.
	bool& at(uint x, uint y);
	bool& at(uint x, uint y, uint channel);
	bool& at(Point p);
	bool& at(Point p, uint channel);

	/// Sets element to 'true' with range checking (does nothing, if out of range).
	/// Invalidates internal cache.
	void mask(uint x, uint y);
	void mask(uint x, uint y, uint channel);
	void mask(Point p);
	void mask(Point p, uint channel);

	/// Sets element to 'false' with range checking (does nothing, if out of range).
	/// Invalidates internal cache.
	void unmask(uint x, uint y);
	void unmask(uint x, uint y, uint channel);
	void unmask(Point p);
	void unmask(Point p, uint channel);

	/// Invokes deep copy.
	Mask clone() const;

	/// Invokes deep copy and then invert the cloned mask.
	Mask clone_invert() const;

	/// Invert current mask.
    void invert();

protected:

    virtual void destroy() const;
};

#endif /* MASK_H_ */
