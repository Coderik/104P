/*
 * Image.cpp
 *
 *  Created on: Nov 5, 2012
 *      Author: Vadim Fedorov
 */

#include <stdlib.h>
#include <stdio.h>

#include "headers/image.h"

template <class T>
Image<T>::Image()
{
	_x_size = 0;
	_y_size = 0;
	_points = 0;
}


template <class T>
Image<T>::Image(int x_size, int y_size)
{
	_x_size = x_size;
	_y_size = y_size;

	_points = (T *)malloc(sizeof(T) * x_size * y_size);
}


template <class T>
Image<T>::Image(int x_size,int y_size, T value)
{
	_x_size = x_size;
	_y_size = y_size;

	_points = (T *)malloc(sizeof(T) * x_size * y_size);

	fill(value);
}


template <class T>
Image<T>::Image(Image<T> &source)
{
	_x_size = source._x_size;
	_y_size = source._y_size;
	_points = (T *)malloc(sizeof(T) * _x_size * _y_size);

	// TODO: rewrite copying
	for (int i = 0; i < _x_size * _y_size; i++) {
		_points[i] = source._points[i];
	}
}


template <class T>
Image<T>::~Image()
{
	if (_points) {
		free(_points);
	}
}

/* public */
template <class T>
T Image<T>::get_value(int x, int y) const
{
	if ( x < 0 || y < 0 || x >= _x_size || y >= _y_size || !_points) {
		return T();
	}

	return _points[GetIndex(x,y)];
}


template <class T>
bool Image<T>::try_get_value(int x, int y, T& value) const
{
	if ( x < 0 || y < 0 || x >= _x_size || y >= _y_size || !_points) {
		return false;
	}

	value = _points[GetIndex(x,y)];
	return true;
}


template <class T>
void Image<T>::set_value(int x, int y, T value)
{
	if ( x < 0 || y < 0 || x >= _x_size || y >= _y_size || !_points) {
		return;
	}

	_points[GetIndex(x,y)] = value;
}


template <class T>
void Image<T>::fill(T value)
{
	std::fill_n(_points, _x_size*_y_size, value);
}


template <class T>
int Image<T>::GetXSize() const
{
	return _x_size;
}


template <class T>
int Image<T>::GetYSize() const
{
	return _y_size;
}


template <class T>
Shape Image<T>::get_size() const
{
	return Shape(_x_size, _y_size, 1);
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
Image<T>* Image<T>::GetPatchBetweenPoints(int a_x, int a_y, int b_x, int b_y)
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
	if (a_x < 0 || b_x >= _x_size ||
		a_y < 0 || b_y >= _y_size) {
		return 0;
	}

	return GetPatchInternal(a_x, a_y, b_x, b_y);
}


template <class T>
Image<T>* Image<T>::GetPatchArountPoint(int center_x, int center_y, int x_size, int y_size)
{
	if (x_size <= 0 || y_size <= 0) {
		return 0;
	}

	// force 'x_size' and 'y_size' to be odd
	if (!IsOddNumber(x_size)) x_size--;
	if (!IsOddNumber(y_size)) y_size--;

	// calculate left-top and bottom-right points of the patch
	int x_offset = x_size / 2;
	int y_offset = y_size / 2;
	int a_x = center_x - x_offset;
	int a_y = center_y - y_offset;
	int b_x = center_x + x_offset;
	int b_y = center_y + y_offset;

	// check if whole patch is inside the image
	if (a_x < 0 || b_x >= _x_size ||
		a_y < 0 || b_y >= _y_size) {
		return 0;
	}

	return GetPatchInternal(a_x, a_y, b_x, b_y);
}


template <class T>
Image<T>* Image<T>::GetPatchArountPoint(int center_x, int center_y, int size)
{
	return GetPatchArountPoint(center_x, center_y, size, size);
}


template <class T>
int Image<T>::GetRawDataLength()
{
	return _x_size * _y_size;
}


/**
 * Returns pointer to internal raw data representation.
 */
 template <class T>
T* Image<T>::GetRawData()
{
	return _points;
}


/* private */
template <class T>
inline int Image<T>::GetIndex(int x, int y) const
{
	return _x_size * y + x;
}


template <class T>
inline bool Image<T>::IsOddNumber(int number)
{
	return number % 2 == 1;
}

template <class T>
Image<T>* Image<T>::GetPatchInternal(int a_x, int a_y, int b_x, int b_y)
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

// Explicit instantiations
template class Image<float>;
template class Image<bool>;
template class Image<Point>;
