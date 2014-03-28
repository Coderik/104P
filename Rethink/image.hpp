/*
 * image.hpp
 *
 *  Created on: 27 марта 2014 г.
 *      Author: Coderik
 */

#include "image.h"

template <class T>
Image<T>::Image()
 : _size_x(0), _size_y(0), _number_of_channels(0), _data(0), _ref(0)
{
}


template <class T>
Image<T>::Image(uint size_x, uint size_y)
 : _size_x(size_x), _size_y(size_y), _number_of_channels(1)
{
	init(size_x, size_y, 1);
}


template <class T>
Image<T>::Image(uint size_x, uint size_y, uint number_of_channels)
 : _size_x(size_x), _size_y(size_y), _number_of_channels(number_of_channels)
{
	init(size_x, size_y, number_of_channels);
}


template <class T>
Image<T>::Image(uint size_x, uint size_y, T default_value)
 : _size_x(size_x), _size_y(size_y), _number_of_channels(1)
{
	init(size_x, size_y, 1);
	fill(default_value);
}


template <class T>
Image<T>::Image(uint size_x, uint size_y, uint number_of_channels, T default_value)
 : _size_x(size_x), _size_y(size_y), _number_of_channels(number_of_channels)
{
	init(size_x, size_y, number_of_channels);
	fill(default_value);
}


template <class T>
Image<T>::Image(Shape size)
 : _size_x(size.size_x), _size_y(size.size_y), _number_of_channels(1)
{
	init(size.size_x, size.size_y, 1);
}


template <class T>
Image<T>::Image(Shape size, uint number_of_channels)
 : _size_x(size.size_x), _size_y(size.size_y), _number_of_channels(number_of_channels)
{
	init(size.size_x, size.size_y, number_of_channels);
}


template <class T>
Image<T>::Image(Shape size, T default_value)
 : _size_x(size.size_x), _size_y(size.size_y), _number_of_channels(1)
{
	init(size.size_x, size.size_y, 1);
	fill(default_value);
}


template <class T>
Image<T>::Image(Shape size, uint number_of_channels, T default_value)
 : _size_x(size.size_x), _size_y(size.size_y), _number_of_channels(number_of_channels)
{
	init(size.size_x, size.size_y, number_of_channels);
	fill(default_value);
}


template <class T>
Image<T>::Image(const Image<T> &source)
 : _size_x(source._size_x), _size_y(source._size_y), _number_of_channels(source._number_of_channels), _ref(source._ref), _data(source._data)
{
	if (_ref) {
		_ref->counter++;
	}
}


template <class T>
Image<T>::~Image()
{
	release();
}


template <class T>
const Image<T>& Image<T>::operator= (const Image<T> &other)
{
	// check for self-assignment
	if(this == &other) {
		return *this;
	}

	// finish all deals with the previous data
	release();

	// increase counter
	if (other._ref) {
		other._ref->counter++;
	}

	// assign new data
	_ref = other._ref;
	_size_x = other._size_x;
	_size_y = other._size_y;
	_number_of_channels = other._number_of_channels;
	_data = other._data;

	return *this;
}


template <class T>
Image<T> Image<T>::clone() const
{
	Image<T> clone;
	clone._size_x = _size_x;
	clone._size_y = _size_y;
	clone._number_of_channels = _number_of_channels;
	clone.init(_size_x, _size_y, _number_of_channels);
	memcpy(clone._data, _data,  _number_of_channels * _size_y * _size_x * sizeof(T));

	return clone;
}


template <class T>
uint Image<T>::get_size_x() const
{
	return _size_x;
}


template <class T>
uint Image<T>::get_size_y() const
{
	return _size_y;
}


template <class T>
uint Image<T>::get_number_of_channels() const
{
	return _number_of_channels;
}


template <class T>
const T& Image<T>::operator() (uint x, uint y) const
{
	return *(const T*)(_data + get_index(x, y, 0));
}


template <class T>
const T& Image<T>::operator() (uint x, uint y, uint channel) const
{
	return *(const T*)(_data + get_index(x, y, channel));
}


template <class T>
const T& Image<T>::operator() (Point p) const
{
	return *(const T*)(_data + get_index(p.x, p.y, 0));
}


template <class T>
const T& Image<T>::operator() (Point p, uint channel) const
{
	return *(const T*)(_data + get_index(p.x, p.y, channel));
}


template <class T>
T& Image<T>::operator() (uint x, uint y)
{
	return _data[get_index(x, y, 0)];
}


template <class T>
T& Image<T>::operator() (uint x, uint y, uint channel)
{
	return _data[get_index(x, y, channel)];
}


template <class T>
T& Image<T>::operator() (Point p)
{
	return _data[get_index(p.x, p.y, 0)];
}


template <class T>
T& Image<T>::operator() (Point p, uint channel)
{
	return _data[get_index(p.x, p.y, channel)];
}


template <class T>
const T& Image<T>::at(uint x, uint y) const
{
	if (x >= _size_x || y >= _size_y) {
		throw std::out_of_range();
	}

	return (const T*)(_data + get_index(x, y, 0));
}


template <class T>
const T& Image<T>::at(uint x, uint y, uint channel) const
{
	if (x >= _size_x || y >= _size_y || channel >= _number_of_channels) {
		throw std::out_of_range();
	}

	return (const T*)(_data + get_index(x, y, channel));
}


template <class T>
const T& Image<T>::at(Point p) const
{
	if (p.x >= _size_x || p.y >= _size_y) {
		throw std::out_of_range();
	}

	return (const T*)(_data + get_index(p.x, p.y, 0));
}


template <class T>
const T& Image<T>::at(Point p, uint channel) const
{
	if (p.x >= _size_x || p.y >= _size_y || channel >= _number_of_channels) {
		throw std::out_of_range();
	}

	return (const T*)(_data + get_index(p.x, p.y, channel));
}


template <class T>
T& Image<T>::at(uint x, uint y)
{
	if (x >= _size_x || y >= _size_y) {
		throw std::out_of_range();
	}

	return (T*)(_data + get_index(x, y, 0));
}


template <class T>
T& Image<T>::at(uint x, uint y, uint channel)
{
	if (x >= _size_x || y >= _size_y || channel >= _number_of_channels) {
		throw std::out_of_range();
	}

	return (T*)(_data + get_index(x, y, channel));
}


template <class T>
T& Image<T>::at(Point p)
{
	if (p.x >= _size_x || p.y >= _size_y) {
		throw std::out_of_range();
	}

	return (T*)(_data + get_index(p.x, p.y, 0));
}


template <class T>
T& Image<T>::at(Point p, uint channel)
{
	if (p.x >= _size_x || p.y >= _size_y || channel >= _number_of_channels) {
		throw std::out_of_range();
	}

	return (T*)(_data + get_index(p.x, p.y, channel));
}


template <class T>
bool Image<T>::try_get_value(uint x, uint y, T& value) const
{
	if (x >= _size_x || y >= _size_y || !_data) {
		return false;
	}

	value = _data[get_index(x, y, 0)];

	return true;
}


template <class T>
bool Image<T>::try_get_value(uint x, uint y, uint channel, T& value) const
{
	if (x >= _size_x || y >= _size_y || channel >= _number_of_channels || !_data) {
		return false;
	}

	value = _data[get_index(x, y, channel)];

	return true;
}


template <class T>
bool Image<T>::try_get_value(Point p, T& value) const
{
	if (p.x >= _size_x || p.y >= _size_y || !_data) {
		return false;
	}

	value = _data[get_index(p.x, p.y, 0)];

	return true;
}


template <class T>
bool Image<T>::try_get_value(Point p, uint channel, T& value) const
{
	if (p.x >= _size_x || p.y >= _size_y || channel >= _number_of_channels || !_data) {
		return false;
	}

	value = _data[get_index(p.x, p.y, channel)];

	return true;
}


template <class T>
void Image<T>::fill(const T &value)
{
	std::fill_n(_data, _number_of_channels * _size_y * _size_x, value);
}


template <class T>
const T* Image<T>::raw() const
{
	return _data;
}


template <class T>
T* Image<T>::raw()
{
	return _data;
}

/* Private */

template <class T>
void Image<T>::release()
{
	if (_ref) {
		_ref->counter--;
		if (_ref->counter == 0) {
			delete _ref;
			delete _data;
		}
	}
}


template <class T>
inline void Image<T>::init(uint size_x, uint size_y, uint number_of_channels)
{
	_data = new T[size_x * size_y * number_of_channels];
	_ref = new __Ref();
	_ref->counter = 1;
}


template <class T>
inline uint Image<T>::get_index(uint x, uint y, uint channel) const
{
	return _size_x * (_size_y * channel + y) + x;
}
