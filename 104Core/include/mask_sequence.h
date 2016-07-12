/**
 * Copyright (C) 2016, Vadim Fedorov <coderiks@gmail.com>
 *
 * This program is free software: you can use, modify and/or
 * redistribute it under the terms of the simplified BSD
 * License. You should have received a copy of this license along
 * this program. If not, see
 * <http://www.opensource.org/licenses/bsd-license.html>.
 */

/// Created on: Nov 13, 2014

#ifndef MASK_SEQUENCE_H_
#define MASK_SEQUENCE_H_

#include <vector>
#include "mask.h"
#include "i_iterable_mask.h"

using namespace std;

// forward declaration
class MaskSequenceFx;

class MaskSequence : public IIterableMask
{
friend class MaskSequenceFx;
public:
	typedef MaskIterator iterator;

	MaskSequence();
	MaskSequence(Shape size);
	MaskSequence(uint size_x, uint size_y);
	MaskSequence(uint size_x, uint size_y, uint size_t);
	MaskSequence(uint size_x, uint size_y, uint size_t, bool default_value);
	MaskSequence(const Mask &frame, uint size_t = 1);
	MaskSequence(const MaskFx &frame, uint size_t = 1);
	~MaskSequence();

	MaskSequence(const MaskSequence &other);	// copy constructor
	MaskSequence(const MaskSequenceFx &other);	// copy constructor

	bool add(const Mask &frame);

	uint size_x() const;
	uint size_y() const;
	uint size_t() const;
	Shape size() const;

	operator bool() const;
	bool is_empty() const;

	// NOTE: added for convenience
	bool& operator() (uint x, uint y, uint t);
	bool& operator() (const Point &p);

	/// Returns value without range checking.
	const Mask& operator[] (uint t) const;
	Mask& operator[] (uint t);

	/// Returns value with range checking.
	/// Throws std::out_of_range exception, if out of range.
	const Mask& frame(uint t) const;
	Mask& frame(uint t);

	MaskSequence clone() const;

	/// Invert current mask.
    void invert();

	iterator begin() const;
	iterator end() const;
	iterator rbegin() const;
	iterator rend() const;

	/// Methods used by MaskIterator
	virtual Point first() const;
	virtual Point last() const;
	virtual Point next(const Point &current) const;
	virtual Point prev(const Point &current) const;

private:
	vector<Mask > _frames;
	Shape _size;
};



class MaskSequenceFx : public IIterableMask
{
friend class MaskSequence;
public:
	typedef MaskIterator iterator;

	static const MaskSequenceFx empty;

	MaskSequenceFx();
	MaskSequenceFx(Shape size);
	MaskSequenceFx(uint size_x, uint size_y);
	MaskSequenceFx(const MaskFx &frame, uint size_t = 1);
	MaskSequenceFx(const Mask &frame, uint size_t = 1);
	~MaskSequenceFx();

	MaskSequenceFx(const MaskSequenceFx &other);	// copy constructor
	MaskSequenceFx(const MaskSequence &other);	// copy constructor

	bool add(const MaskFx &frame);

	uint size_x() const;
	uint size_y() const;
	uint size_t() const;
	Shape size() const;

	operator bool() const;
	bool is_empty() const;

	// NOTE: added for convenience
	const bool& operator() (uint x, uint y, uint t) const;
	const bool& operator() (const Point &p) const;

	/// Returns value without range checking.
	const MaskFx& operator[] (uint t) const;
	MaskFx& operator[] (uint t);

	/// Returns value with range checking.
	/// Throws std::out_of_range exception, if out of range.
	const MaskFx& frame(uint t) const;
	MaskFx& frame(uint t);

	MaskSequenceFx clone() const;

	iterator begin() const;
	iterator end() const;
	iterator rbegin() const;
	iterator rend() const;

	/// Methods used by MaskIterator
	virtual Point first() const;
	virtual Point last() const;
	virtual Point next(const Point &current) const;
	virtual Point prev(const Point &current) const;

private:
	vector<MaskFx > _frames;
	Shape _size;
};

#endif /* MASK_SEQUENCE_H_ */
