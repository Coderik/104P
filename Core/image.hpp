/*
 * Image.hpp
 *
 *  Created on: Nov 5, 2012
 *      Author: Vadim Fedorov
 */

#include <stdlib.h>
#include <stdio.h>

template <class T>
Image<T>::Image()
{
	_size_x = 0;
	_size_y = 0;
	_points = 0;
}


// TODO: Possible issue. _points contains rubbish after memory allocation
template <class T>
Image<T>::Image(int size_x, int size_y)
{
	_size_x = size_x;
	_size_y = size_y;

	_points = new T[size_x * size_y];
}


template <class T>
Image<T>::Image(int size_x,int size_y, T value)
{
	_size_x = size_x;
	_size_y = size_y;

	_points = new T[size_x * size_y];

	fill(value);
}


template <class T>
Image<T>::Image(const Image<T> &source)
{
	_size_x = source._size_x;
	_size_y = source._size_y;
	_points = new T[_size_x * _size_y];

	// TODO: rewrite copying
	for (int i = 0; i < _size_x * _size_y; i++) {
		_points[i] = source._points[i];
	}
}


template <class T>
Image<T>::~Image()
{
	if (_points) {
		delete[] _points;
	}
}

/* public */
template <class T>
T Image<T>::get_value(int x, int y) const
{
	if ( x < 0 || y < 0 || x >= _size_x || y >= _size_y || !_points) {
		return T();
	}

	return _points[get_index(x, y)];
}


template <class T>
T Image<T>::get_value(Point p) const
{
	if ( p.x < 0 || p.y < 0 || p.x >= _size_x || p.y >= _size_y || !_points) {
		return T();
	}

	return _points[get_index(p.x, p.y)];
}


template <class T>
bool Image<T>::try_get_value(int x, int y, T& value) const
{
	if ( x < 0 || y < 0 || x >= _size_x || y >= _size_y || !_points) {
		return false;
	}

	value = _points[get_index(x,y)];
	return true;
}


template <class T>
void Image<T>::set_value(int x, int y, T value)
{
	if ( x < 0 || y < 0 || x >= _size_x || y >= _size_y || !_points) {
		return;
	}

	_points[get_index(x, y)] = value;
}


template <class T>
void Image<T>::set_value(Point p, T value)
{
	if ( p.x < 0 || p.y < 0 || p.x >= _size_x || p.y >= _size_y || !_points) {
		return;
	}

	_points[get_index(p.x, p.y)] = value;
}


template <class T>
void Image<T>::fill(T value)
{
	std::fill_n(_points, _size_x*_size_y, value);
}


template <class T>
int Image<T>::get_size_x() const
{
	return _size_x;
}


template <class T>
int Image<T>::get_size_y() const
{
	return _size_y;
}


template <class T>
Shape Image<T>::get_size() const
{
	return Shape(_size_x, _size_y, 1);
}


template <class T>
void Image<T>::set_coordinates(Point coordinates)
{
	_coordinates = coordinates;
}


template <class T>
Point Image<T>::get_coordinates()
{
	return _coordinates;
}


template <class T>
Image<T>* Image<T>::get_patch_between_points(int a_x, int a_y, int b_x, int b_y)
{
	// /* ensure that points lie inside the image */
	// a_x = std::max(std::min(a_x, _x_size - 1), 0);
	// a_y = std::max(std::min(a_y, _x_size - 1), 0);
	// b_x = std::max(std::min(b_x, _x_size - 1), 0);
	// b_y = std::max(std::min(b_y, _x_size - 1), 0);

	if (a_x >= b_x || a_y >= b_y) {
		return 0;
	}

	// check if whole patch is inside the image
	if (a_x < 0 || b_x >= _size_x ||
		a_y < 0 || b_y >= _size_y) {
		return 0;
	}

	return get_patch_internal(a_x, a_y, b_x, b_y);
}


template <class T>
Image<T>* Image<T>::get_patch_around_point(int center_x, int center_y, int size_x, int size_y)
{
	if (size_x <= 0 || size_y <= 0) {
		return 0;
	}

	// force 'x_size' and 'y_size' to be odd
	if (!is_odd_number(size_x)) size_x--;
	if (!is_odd_number(size_y)) size_y--;

	// calculate left-top and bottom-right points of the patch
	int x_offset = size_x / 2;
	int y_offset = size_y / 2;
	int a_x = center_x - x_offset;
	int a_y = center_y - y_offset;
	int b_x = center_x + x_offset;
	int b_y = center_y + y_offset;

	// check if whole patch is inside the image
	if (a_x < 0 || b_x >= _size_x ||
		a_y < 0 || b_y >= _size_y) {
		return 0;
	}

	return get_patch_internal(a_x, a_y, b_x, b_y);
}


template <class T>
Image<T>* Image<T>::get_patch_around_point(int center_x, int center_y, int size)
{
	return get_patch_around_point(center_x, center_y, size, size);
}


template <class T>
int Image<T>::get_raw_data_length() const
{
	return _size_x * _size_y;
}


/**
 * Returns pointer to internal raw data representation.
 */
 template <class T>
const T* Image<T>::get_raw_data() const
{
	return _points;
}


/**
* Returns pointer to internal raw data representation.
*/
template <class T>
T* Image<T>::get_raw_data()
{
return _points;
}


/* private */
template <class T>
inline int Image<T>::get_index(int x, int y) const
{
	return _size_x * y + x;
}


template <class T>
inline bool Image<T>::is_odd_number(int number)
{
	return number % 2 == 1;
}

template <class T>
Image<T>* Image<T>::get_patch_internal(int a_x, int a_y, int b_x, int b_y)
{
	int x_size = b_x - a_x + 1;
	int y_size = b_y - a_y + 1;

	Image<T> *patch = new Image<T>(x_size, y_size);

	// TODO: may be add mirror effect at the border?
	for (int x = 0; x < x_size; x++) {
		for (int y = 0; y < y_size; y++) {
			T value = this->get_value(a_x + x, a_y + y);
			patch->set_value(x,y,value);
		}
	}

	return patch;
}
