/*
 * image_mask.cpp
 *
 *  Created on: Apr 20, 2013
 *      Author: Vadim Fedorov
 */

#include "headers/image_mask.h"

ImageMask::ImageMask()
	: Image<bool>()
{

}


ImageMask::ImageMask(int x_size,int y_size)
	: Image<bool>(x_size, y_size, false)
{

}


ImageMask::ImageMask(int x_size,int y_size, bool value)
	: Image<bool>(x_size, y_size, value)
{

}


/**
 * Deep copy
 */
ImageMask::ImageMask(Image<bool> &source)
	: Image<bool>(source)
{

}


/**
 * Deep copy
 */
ImageMask::ImageMask(ImageMask &source)
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
	for (int y = 0; y < _y_size; y++) {
		for (int x = 0; x < _x_size; x++) {
			set_value(x, y, !get_value(x, y));
		}
	}
}


/* I_Iterable_Mask methods */
// TODO: optimize
Point ImageMask::first() const
{
	for (int y = 0; y < _y_size; y++) {
		for (int x = 0; x < _x_size; x++) {
			if (get_value(x, y)) {
				return Point(x, y, 0);
			}
		}
	}

	return Point(-1, -1, -1);
}


Point ImageMask::last() const
{
	for (int y = _y_size - 1; y >= 0; y--) {
		for (int x = _x_size - 1; x >= 0; x--) {
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
	for (int y = current.y; y < _y_size; y++) {
		for (int x = from_x; x < _x_size; x++) {
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
		from_x = _x_size - 1;
	}

	return Point(-1, -1, -1);
}
