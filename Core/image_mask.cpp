/*
 * image_mask.cpp
 *
 *  Created on: Apr 20, 2013
 *      Author: Vadim Fedorov
 */

#include "image_mask.h"

ImageMask::ImageMask()
	: Image<bool>()
{

}


ImageMask::ImageMask(int size_x,int size_y)
	: Image<bool>(size_x, size_y, false)
{

}


ImageMask::ImageMask(int size_x,int size_y, bool value)
	: Image<bool>(size_x, size_y, value)
{

}


/**
 * Deep copy
 */
ImageMask::ImageMask(const Image<bool> &source)
	: Image<bool>(source)
{

}


/**
 * Deep copy
 */
ImageMask::ImageMask(const ImageMask &source)
	: Image<bool>(source)
{

}


ImageMask::~ImageMask()
{

}


ImageMask::iterator ImageMask::begin() const
{
	return iterator(this, first());
}


ImageMask::iterator ImageMask::end() const
{
	return iterator(this, Point(-1, -1, -1));
}


ImageMask::iterator ImageMask::rbegin() const
{
	return iterator(this, last(), true);
}


ImageMask::iterator ImageMask::rend() const
{
	return iterator(this, Point(-1, -1, -1), true);
}


void ImageMask::mask(int x, int y)
{
	set_value(x, y, true);
}


void ImageMask::unmask(int x, int y)
{
	set_value(x, y, false);
}


void ImageMask::invert()
{
	// TODO: may be move to specialization Image<bool>
	for (int y = 0; y < _size_y; y++) {
		for (int x = 0; x < _size_x; x++) {
			set_value(x, y, !get_value(x, y));
		}
	}
}


/* I_Iterable_Mask methods */
// TODO: optimize
Point ImageMask::first() const
{
	for (int y = 0; y < _size_y; y++) {
		for (int x = 0; x < _size_x; x++) {
			if (get_value(x, y)) {
				return Point(x, y, 0);
			}
		}
	}

	return Point(-1, -1, -1);
}


Point ImageMask::last() const
{
	for (int y = _size_y - 1; y >= 0; y--) {
		for (int x = _size_x - 1; x >= 0; x--) {
			if (get_value(x, y)) {
				return Point(x, y, 0);
			}
		}
	}

	return Point(-1, -1, -1);
}


Point ImageMask::next(const Point current) const
{
	int from_x = current.x + 1;
	for (int y = current.y; y < _size_y; y++) {
		for (int x = from_x; x < _size_x; x++) {
			if (get_value(x, y)) {
				return Point(x, y, 0);
			}
		}
		from_x = 0;
	}

	return Point(-1, -1, -1);
}


Point ImageMask::prev(const Point current) const
{
	int from_x = current.x - 1;
	for (int y = current.y; y >= 0; y--) {
		for (int x = from_x; x >= 0; x--) {
			if (get_value(x, y)) {
				return Point(x, y, 0);
			}
		}
		from_x = _size_x - 1;
	}

	return Point(-1, -1, -1);
}
