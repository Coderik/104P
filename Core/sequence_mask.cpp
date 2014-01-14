/*
 * sequence_mask.cpp
 *
 *  Created on: Apr 20, 2013
 *      Author: Vadim Fedorov
 */

#include "sequence_mask.h"

SequenceMask::SequenceMask(int size_x, int size_y, int size_t, bool value)
	: Sequence<bool>(size_x, size_y, size_t, value)
{

}


SequenceMask::SequenceMask(int size_x, int size_y, int size_t)
	: Sequence<bool>(size_x, size_y, size_t, false)
{

}


SequenceMask::SequenceMask(int size_x, int size_y)
	: Sequence<bool>(size_x, size_y)
{

}


SequenceMask::SequenceMask(Image<bool> *first_frame)
	: Sequence<bool>(first_frame)
{

}


SequenceMask::SequenceMask(SequenceMask& source)
	: Sequence<bool>(source)
{

}


SequenceMask::~SequenceMask()
{

}


ImageMask* SequenceMask::get_mask_frame(int t) const
{
	if (t < 0 || t >= _size_t || !_frames[t]) {
		return 0;
	}

	return new ImageMask(*_frames[t]);
}


SequenceMask::iterator SequenceMask::begin() const
{
	return iterator(this, first());
}


SequenceMask::iterator SequenceMask::end() const
{
	return iterator(this, Point(-1, -1, -1));
}


SequenceMask::iterator SequenceMask::rbegin() const
{
	return iterator(this, last(), true);
}


SequenceMask::iterator SequenceMask::rend() const
{
	return iterator(this, Point(-1, -1, -1), true);
}


void SequenceMask::mask(int x, int y, int t)
{
	set_value(x, y, t, true);
}


void SequenceMask::unmask(int x, int y, int t)
{
	set_value(x, y, t, false);
}


void SequenceMask::invert()
{
	// TODO: may be move to specialization Image<bool>
	for (int t = 0; t < _size_t; t++) {
		for (int y = 0; y < _size_y; y++) {
			for (int x = 0; x < _size_x; x++) {
				set_value(x, y, t, !get_value(x, y, t));
			}
		}
	}
}


/* I_Iterable_Mask methods */
// TODO: optimize
Point SequenceMask::first() const
{
	for (int t = 0; t < _size_t; t++) {
		for (int y = 0; y < _size_y; y++) {
			for (int x = 0; x < _size_x; x++) {
				if (get_value(x, y, t)) {
					return Point(x, y, t);
				}
			}
		}
	}

	return Point(-1, -1, -1);
}


Point SequenceMask::last() const
{
	for (int t = _size_t - 1; t >= 0; t--) {
		for (int y = _size_y - 1; y >= 0; y--) {
			for (int x = _size_x - 1; x >= 0; x--) {
				if (get_value(x, y, t)) {
					return Point(x, y, t);
				}
			}
		}
	}

	return Point(-1, -1, -1);
}


Point SequenceMask::next(const Point current) const
{
	int from_x = current.x + 1;
	int from_y = current.y;
	for (int t = current.t; t < _size_t; t++) {
		for (int y = from_y; y < _size_y; y++) {
			for (int x = from_x; x < _size_x; x++) {
				if (get_value(x, y, t)) {
					return Point(x, y, t);
				}
			}
			from_x = 0;
		}
		from_y = 0;
	}

	return Point(-1, -1, -1);
}


Point SequenceMask::prev(const Point current) const
{
	int from_x = current.x - 1;
	int from_y = current.y;
	for (int t = current.t; t >= 0; t--) {
		for (int y = from_y; y >= 0; y--) {
			for (int x = from_x; x >= 0; x--) {
				if (get_value(x, y, t)) {
					return Point(x, y, t);
				}
			}
			from_x = _size_x - 1;
		}
		from_y = _size_y - 1;
	}

	return Point(-1, -1, -1);
}


SequenceMask::iterator SequenceMask::empty_iterator()
{
	return iterator(0, Point(-1, -1, -1));
}
