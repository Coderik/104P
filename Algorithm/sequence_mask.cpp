/*
 * sequence_mask.cpp
 *
 *  Created on: Apr 20, 2013
 *      Author: Vadim Fedorov
 */

#include "headers/sequence_mask.h"

SequenceMask::SequenceMask(int x_size, int y_size, int t_size, bool value)
	: Sequence<bool>(x_size, y_size, t_size, value)
{

}


SequenceMask::SequenceMask(int x_size, int y_size, int t_size)
	: Sequence<bool>(x_size, y_size, t_size, false)
{

}


SequenceMask::SequenceMask(int x_size, int y_size)
	: Sequence<bool>(x_size, y_size)
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


SequenceMask::iterator SequenceMask::begin()
{
	return iterator(this, first());
}


SequenceMask::iterator SequenceMask::end()
{
	return iterator(this, Point(-1, -1, -1));
}


SequenceMask::iterator SequenceMask::rbegin()
{
	return iterator(this, last());
}


SequenceMask::iterator SequenceMask::rend()
{
	return iterator(this, Point(-1, -1, -1));
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
	for (int t = 0; t < _t_size; t++) {
		for (int y = 0; y < _y_size; y++) {
			for (int x = 0; x < _x_size; x++) {
				set_value(x, y, t, !get_value(x, y, t));
			}
		}
	}
}


/* I_Iterable_Mask methods */
// TODO: optimize
Point SequenceMask::first()
{
	for (int t = 0; t < _t_size; t++) {
		for (int y = 0; y < _y_size; y++) {
			for (int x = 0; x < _x_size; x++) {
				if (get_value(x, y, t)) {
					return Point(x, y, t);
				}
			}
		}
	}

	return Point(-1, -1, -1);
}


Point SequenceMask::last()
{
	for (int t = _t_size - 1; t >= 0; t--) {
		for (int y = _y_size - 1; y >= 0; y--) {
			for (int x = _x_size - 1; x >= 0; x--) {
				if (get_value(x, y, t)) {
					return Point(x, y, t);
				}
			}
		}
	}

	return Point(-1, -1, -1);
}


Point SequenceMask::next(const Point current)
{
	int from_x = current.x + 1;
	int from_y = current.y;
	for (int t = current.t; t < _t_size; t++) {
		for (int y = from_y; y < _y_size; y++) {
			for (int x = from_x; x < _x_size; x++) {
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


Point SequenceMask::prev(const Point current)
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
			from_x = _x_size - 1;
		}
		from_y = _y_size - 1;
	}

	return Point(-1, -1, -1);
}
