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
	_first = Point(0, 0);
	_last = Point(-1, -1);
	_is_first_last_valid = true;
	_is_points_cache_valid = false;
}


ImageMask::ImageMask(int size_x,int size_y)
	: Image<bool>(size_x, size_y, false)
{
	_first = Point(size_x, size_y);
	_last = Point(-1, -1);
	_is_first_last_valid = true;
	_is_points_cache_valid = false;
}


ImageMask::ImageMask(int size_x,int size_y, bool value)
	: Image<bool>(size_x, size_y, value)
{
	if (value) {
		_first = Point(0, 0);
		_last = Point(size_x - 1, size_y - 1);
	} else {
		_first = Point(size_x, size_y);
		_last = Point(-1, -1);
	}

	_is_first_last_valid = true;
	_is_points_cache_valid = false;
}


/**
 * Deep copy
 */
ImageMask::ImageMask(const Image<bool> &source)
	: Image<bool>(source)
{
	_is_first_last_valid = false;
	_is_points_cache_valid = false;
}


/**
 * Deep copy
 */
ImageMask::ImageMask(const ImageMask &source)
	: Image<bool>(source)
{
	_first = source._first;
	_last = source._last;
	_is_first_last_valid = source._is_first_last_valid;

	_is_points_cache_valid = false;	// Note: points cache is not copied
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
	return iterator(this, Point(_size_x, _size_y));
}


ImageMask::iterator ImageMask::rbegin() const
{
	return iterator(this, last(), true);
}


ImageMask::iterator ImageMask::rend() const
{
	return iterator(this, Point(-1, -1), true);
}


void ImageMask::set_value(int x, int y, bool value)
 {
	if ( x < 0 || y < 0 || x >= _size_x || y >= _size_y || !_points) {
		return;
	}

	_points[get_index(x, y)] = value;

	_is_first_last_valid = false;
	_is_points_cache_valid = false;

	/*_is_first_last_valid = _is_first_last_valid &&
		((y > _first.y) || (y == _first.y && x > _first.x)) &&
		((y < _last.y) || (y == _last.y && x < _last.x));*/
}


void ImageMask::set_value(Point p, bool value)
{
	if ( p.x < 0 || p.y < 0 || p.x >= _size_x || p.y >= _size_y || !_points) {
		return;
	}

	_points[get_index(p.x, p.y)] = value;

	_is_first_last_valid = false;
	_is_points_cache_valid = false;
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
	for (int y = 0; y < _size_y; y++) {
		for (int x = 0; x < _size_x; x++) {
			set_value(x, y, !get_value(x, y));
		}
	}
}



vector<Point> ImageMask::get_masked_points() const
{
	if (!_is_points_cache_valid) {
		_points_cache.clear();

		// fill points cache
		for(iterator it = begin(); it != end(); ++it) {
			_points_cache.push_back(*it);
		}

		_is_points_cache_valid = true;
	}

	return _points_cache;
}


/* I_Iterable_Mask methods */
Point ImageMask::first() const
{
	if (!_is_first_last_valid) {
		actualize_first_last();
	}

	return _first;
}


Point ImageMask::last() const
{
	if (!_is_first_last_valid) {
		actualize_first_last();
	}

	return _last;
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

	return Point(_size_x, _size_y);
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


/* Private */

inline void ImageMask::actualize_first_last() const
{
	_first = Point(_size_x, _size_y);
	_last = Point(-1, -1);

	bool _is_last_found = false;
	for (int y = _size_y - 1; (!_is_last_found) && (y >= 0); y--) {
		for (int x = _size_x - 1; (!_is_last_found) && (x >= 0); x--) {
			if (get_value(x, y)) {
				_last = Point(x, y);
				_is_last_found = true;
			}
		}
	}

	if (_is_last_found) {
		bool _is_first_found = false;
		for (int y = 0; (!_is_first_found) && (y < _size_y); y++) {
			for (int x = 0; (!_is_first_found) && (x < _size_x); x++) {
				if (get_value(x, y)) {
					_first = Point(x, y);
					_is_first_found = true;
				}
			}
		}
	}

  _is_first_last_valid = true;
}
