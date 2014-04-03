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
	init(Point(0, 0), Point(-1, -1), true);
}


ImageMask::ImageMask(uint size_x, uint size_y)
	: Image<bool>(size_x, size_y, false)
{
	init(Point(size_x, size_y), Point(-1, -1), true);
}


ImageMask::ImageMask(uint size_x, uint size_y, bool default_value)
	: Image<bool>(size_x, size_y, default_value)
{
	if (default_value) {
		init(Point(0, 0), Point(size_x - 1, size_y - 1), true);
	} else {
		init(Point(size_x, size_y), Point(-1, -1), true);
	}
}


ImageMask::ImageMask(Shape size)
	: Image<bool>(size, false)
{
	init(Point(size.size_x, size.size_y), Point(-1, -1), true);
}


ImageMask::ImageMask(Shape size, bool default_value)
: Image<bool>(size.size_x, size.size_y, default_value)
{
	if (default_value) {
		init(Point(0, 0), Point(size.size_x - 1, size.size_y - 1), true);
	} else {
		init(Point(size.size_x, size.size_y), Point(-1, -1), true);
	}
}


/**
 * Deep copy
 */
ImageMask::ImageMask(const Image<bool> &source)
	: Image<bool>(source.clone())	// here we need to invoke deep copying explicitly, because normally Image to Image transition leads to the data sharing
{
	init(Point(0, 0), Point(0, 0), false);
}


/**
 * Deep copy
 */
ImageMask::ImageMask(const ConstImage<bool> &source)
	: Image<bool>(source)	// here we have an implicit deep copying, because it is mandatory for ConstImage to Image transition
{
	init(Point(0, 0), Point(0, 0), false);
}


/**
 * Ref++
 */
ImageMask::ImageMask(const ImageMask &source)
	: Image<bool>(source)	// ImageMask is casted to Image<bool> implicitly, then Image to Image transition leads to the data sharing
{
	_internal = source._internal;
}


/**
 * Deep copy
 */
/*ImageMask::ImageMask(const ConstImageMask &source)
	: Image<bool>(source)	// ConstImageMask is casted to ConstImage<bool> implicitly, then ConstImage to Image transition leads to the implicit deep copying
{
	_first = source._first;
	_last = source._last;
	_is_first_last_valid = source._is_first_last_valid;

	_is_points_cache_valid = false;	// Note: points cache is not copied
}*/


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


bool& ImageMask::operator() (uint x, uint y)
{
	_internal->is_first_last_valid = false;
	_internal->is_points_cache_valid = false;

	return _data[get_index(x, y, 0)];
}


bool& ImageMask::operator() (uint x, uint y, uint channel)
{
	_internal->is_first_last_valid = false;
	_internal->is_points_cache_valid = false;

	return _data[get_index(x, y, channel)];
}


bool& ImageMask::operator() (Point p)
{
	_internal->is_first_last_valid = false;
	_internal->is_points_cache_valid = false;

	return _data[get_index(p.x, p.y, 0)];
}


bool& ImageMask::operator() (Point p, uint channel)
{
	_internal->is_first_last_valid = false;
	_internal->is_points_cache_valid = false;

	return _data[get_index(p.x, p.y, channel)];
}


bool& ImageMask::at(uint x, uint y)
{
	if (x >= _size_x || y >= _size_y) {
		throw std::out_of_range("");				// TODO: add exception messages everywhere
	}

	_internal->is_first_last_valid = false;
	_internal->is_points_cache_valid = false;

	return _data[get_index(x, y, 0)];
}


bool& ImageMask::at(uint x, uint y, uint channel)
{
	if (x >= _size_x || y >= _size_y || channel >= _number_of_channels) {
		throw std::out_of_range("");
	}

	_internal->is_first_last_valid = false;
	_internal->is_points_cache_valid = false;

	return _data[get_index(x, y, channel)];
}


bool& ImageMask::at(Point p)
{
	if (p.x < 0 || p.x >= _size_x || p.y < 0 || p.y >= _size_y) {
		throw std::out_of_range("");
	}

	_internal->is_first_last_valid = false;
	_internal->is_points_cache_valid = false;

	return _data[get_index(p.x, p.y, 0)];
}


bool& ImageMask::at(Point p, uint channel)
{
	if (p.x < 0 || p.x >= _size_x || p.x < 0 || p.y >= _size_y || channel >= _number_of_channels) {
		throw std::out_of_range("");
	}

	_internal->is_first_last_valid = false;
	_internal->is_points_cache_valid = false;

	return _data[get_index(p.x, p.y, channel)];
}


bool ImageMask::get(uint x, uint y)
{
	return _data[get_index(x, y, 0)];
}


bool ImageMask::get(uint x, uint y, uint channel)
{

	return _data[get_index(x, y, channel)];
}


bool ImageMask::get(Point p)
{
	return _data[get_index(p.x, p.y, 0)];
}


bool ImageMask::get(Point p, uint channel)
{
	return _data[get_index(p.x, p.y, channel)];
}


bool ImageMask::test(uint x, uint y)
{
	if (x >= _size_x || y >= _size_y) {
		return false;
	}

	return _data[get_index(x, y, 0)];
}


bool ImageMask::test(uint x, uint y, uint channel)
{
	if (x >= _size_x || y >= _size_y || channel >= _number_of_channels) {
		return false;
	}

	return _data[get_index(x, y, channel)];
}


bool ImageMask::test(Point p)
{
	if (p.x < 0 || p.x >= _size_x || p.y < 0 || p.y >= _size_y) {
		return false;
	}

	return _data[get_index(p.x, p.y, 0)];
}


bool ImageMask::test(Point p, uint channel)
{
	if (p.x < 0 || p.x >= _size_x || p.x < 0 || p.y >= _size_y || channel >= _number_of_channels) {
		return false;
	}

	return _data[get_index(p.x, p.y, channel)];
}


void ImageMask::mask(uint x, uint y)
{
	if (x >= _size_x || y >= _size_y) {
		return;
	}

	_internal->is_first_last_valid = false;
	_internal->is_points_cache_valid = false;

	_data[get_index(x, y, 0)] = true;
}


void ImageMask::mask(uint x, uint y, uint channel)
{
	if (x >= _size_x || y >= _size_y || channel >= _number_of_channels) {
		return;
	}

	_internal->is_first_last_valid = false;
	_internal->is_points_cache_valid = false;

	_data[get_index(x, y, channel)] = true;
}


void ImageMask::mask(Point p)
{
	if (p.x < 0 || p.x >= _size_x || p.y < 0 || p.y >= _size_y) {
		return;
	}

	_internal->is_first_last_valid = false;
	_internal->is_points_cache_valid = false;

	_data[get_index(p.x, p.y, 0)] = true;
}


void ImageMask::mask(Point p, uint channel)
{
	if (p.x < 0 || p.x >= _size_x || p.x < 0 || p.y >= _size_y || channel >= _number_of_channels) {
		return;
	}

	_internal->is_first_last_valid = false;
	_internal->is_points_cache_valid = false;

	_data[get_index(p.x, p.y, channel)] = true;
}


void ImageMask::unmask(uint x, uint y)
{
	if (x >= _size_x || y >= _size_y) {
		return;
	}

	_internal->is_first_last_valid = false;
	_internal->is_points_cache_valid = false;

	_data[get_index(x, y, 0)] = true;
}


void ImageMask::unmask(uint x, uint y, uint channel)
{
	if (x >= _size_x || y >= _size_y || channel >= _number_of_channels) {
		return;
	}

	_internal->is_first_last_valid = false;
	_internal->is_points_cache_valid = false;

	_data[get_index(x, y, channel)] = true;
}


void ImageMask::unmask(Point p)
{
	if (p.x < 0 || p.x >= _size_x || p.y < 0 || p.y >= _size_y) {
		return;
	}

	_internal->is_first_last_valid = false;
	_internal->is_points_cache_valid = false;

	_data[get_index(p.x, p.y, 0)] = true;
}


void ImageMask::unmask(Point p, uint channel)
{
	if (p.x < 0 || p.x >= _size_x || p.x < 0 || p.y >= _size_y || channel >= _number_of_channels) {
		return;
	}

	_internal->is_first_last_valid = false;
	_internal->is_points_cache_valid = false;

	_data[get_index(p.x, p.y, channel)] = true;
}


void ImageMask::invert()
{
	for (int i = 0; i < _size_x * _size_y * _number_of_channels; i++) {
		_data[i] = !_data[i];
	}

	_internal->is_first_last_valid = false;
	_internal->is_points_cache_valid = false;
}



vector<Point> ImageMask::get_masked_points() const
{
	if (!_internal->is_points_cache_valid) {
		_internal->points_cache.clear();

		// fill points cache
		for(iterator it = begin(); it != end(); ++it) {
			_internal->points_cache.push_back(*it);
		}

		_internal->is_points_cache_valid = true;
	}

	return _internal->points_cache;
}


/* I_Iterable_Mask methods */
Point ImageMask::first() const
{
	if (!_internal->is_first_last_valid) {
		actualize_first_last();
	}

	return _internal->first;
}


Point ImageMask::last() const
{
	if (!_internal->is_first_last_valid) {
		actualize_first_last();
	}

	return _internal->last;
}


Point ImageMask::next(const Point &current) const
{
	int from_x = current.x + 1;
	for (int y = current.y; y < _size_y; y++) {
		for (int x = from_x; x < _size_x; x++) {
			if (_data[get_index(x, y, 0)]) {
				return Point(x, y, 0);
			}
		}
		from_x = 0;
	}

	return Point(_size_x, _size_y);
}


Point ImageMask::prev(const Point &current) const
{
	int from_x = current.x - 1;
	for (int y = current.y; y >= 0; y--) {
		for (int x = from_x; x >= 0; x--) {
			if (_data[get_index(x, y, 0)]) {
				return Point(x, y, 0);
			}
		}
		from_x = _size_x - 1;
	}

	return Point(-1, -1, -1);
}


/* Private */

inline void ImageMask::init(Point first, Point last, bool is_first_last_valid)
{
	_internal = new __Internal();
	_internal->first = first;
	_internal->last = last;
	_internal->is_first_last_valid = is_first_last_valid;
	_internal->is_points_cache_valid = false;
}


inline void ImageMask::actualize_first_last() const
{
	_internal->first = Point(_size_x, _size_y);
	_internal->last = Point(-1, -1);

	bool is_last_found = false;
	for (int y = _size_y - 1; (!is_last_found) && (y >= 0); y--) {
		for (int x = _size_x - 1; (!is_last_found) && (x >= 0); x--) {
			if (_data[get_index(x, y, 0)]) {
				_internal->last = Point(x, y);
				is_last_found = true;
			}
		}
	}

	if (is_last_found) {
		bool _is_first_found = false;
		for (int y = 0; (!_is_first_found) && (y < _size_y); y++) {
			for (int x = 0; (!_is_first_found) && (x < _size_x); x++) {
				if (_data[get_index(x, y, 0)]) {
					_internal->first = Point(x, y);
					_is_first_found = true;
				}
			}
		}
	}

	_internal->is_first_last_valid = true;
}
