/*
 * image_mask.cpp
 *
 *  Created on: Apr 20, 2013
 *      Author: Vadim Fedorov
 */

#include "headers/image_mask.h"

Image_Mask::Image_Mask()
	: Image<bool>()
{

}


Image_Mask::Image_Mask(int x_size,int y_size)
	: Image<bool>(x_size, y_size, false)
{

}


Image_Mask::Image_Mask(int x_size,int y_size, bool value)
	: Image<bool>(x_size, y_size, value)
{

}


Image_Mask::Image_Mask(Image_Mask &source)
	: Image<bool>(source)
{

}


Image_Mask::~Image_Mask()
{

}


Image_Mask::iterator Image_Mask::begin()
{
	return iterator(this, first());
}


Image_Mask::iterator Image_Mask::end()
{
	return iterator(this, Point(-1, -1, -1));
}


Image_Mask::iterator Image_Mask::rbegin()
{
	return iterator(this, last());
}


Image_Mask::iterator Image_Mask::rend()
{
	return iterator(this, Point(-1, -1, -1));
}


void Image_Mask::mask(int x, int y)
{
	SetPixelValue(x, y, true);
}


void Image_Mask::unmask(int x, int y)
{
	SetPixelValue(x, y, false);
}


void Image_Mask::invert()
{
	// TODO: may be move to specialization Image<bool>
	for (int y = 0; y < _y_size; y++) {
		for (int x = 0; x < _x_size; x++) {
			SetPixelValue(x, y, !GetPixelValue(x, y));
		}
	}
}


/* I_Iterable_Mask methods */
// TODO: optimize
Point Image_Mask::first()
{
	for (int y = 0; y < _y_size; y++) {
		for (int x = 0; x < _x_size; x++) {
			if (GetPixelValue(x, y)) {
				return Point(x, y, 0);
			}
		}
	}

	return Point(-1, -1, -1);
}


Point Image_Mask::last()
{
	for (int y = _y_size - 1; y >= 0; y--) {
		for (int x = _x_size - 1; x >= 0; x--) {
			if (GetPixelValue(x, y)) {
				return Point(x, y, 0);
			}
		}
	}

	return Point(-1, -1, -1);
}


Point Image_Mask::next(const Point current)
{
	int from_x = current.x + 1;
	for (int y = current.y; y < _y_size; y++) {
		for (int x = from_x; x < _x_size; x++) {
			if (GetPixelValue(x, y)) {
				return Point(x, y, 0);
			}
		}
		from_x = 0;
	}

	return Point(-1, -1, -1);
}


Point Image_Mask::prev(const Point current)
{
	int from_x = current.x - 1;
	for (int y = current.y; y >= 0; y--) {
		for (int x = from_x; x >= 0; x--) {
			if (GetPixelValue(x, y)) {
				return Point(x, y, 0);
			}
		}
		from_x = _x_size - 1;
	}

	return Point(-1, -1, -1);
}