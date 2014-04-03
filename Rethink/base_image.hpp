/*
 * base_BaseImage.hpp
 *
 *  Created on: 31 марта 2014 г.
 *      Author: Coderik
 */

#include "base_image.h"

template <class T>
BaseImage<T>::BaseImage()
 : _size_x(0), _size_y(0), _number_of_channels(0), _data(0), _ref(0)
{
}


template <class T>
BaseImage<T>::BaseImage(uint size_x, uint size_y)
 : _size_x(size_x), _size_y(size_y), _number_of_channels(1)
{
	init(size_x, size_y, 1);
}


template <class T>
BaseImage<T>::BaseImage(uint size_x, uint size_y, uint number_of_channels)
 : _size_x(size_x), _size_y(size_y), _number_of_channels(number_of_channels)
{
	init(size_x, size_y, number_of_channels);
}


template <class T>
BaseImage<T>::BaseImage(uint size_x, uint size_y, T default_value)
 : _size_x(size_x), _size_y(size_y), _number_of_channels(1)
{
	init(size_x, size_y, 1);
	fill_internal(default_value);
}


template <class T>
BaseImage<T>::BaseImage(uint size_x, uint size_y, uint number_of_channels, T default_value)
 : _size_x(size_x), _size_y(size_y), _number_of_channels(number_of_channels)
{
	init(size_x, size_y, number_of_channels);
	fill_internal(default_value);
}


template <class T>
BaseImage<T>::BaseImage(Shape size)
 : _size_x(size.size_x), _size_y(size.size_y), _number_of_channels(1)
{
	init(size.size_x, size.size_y, 1);
}


template <class T>
BaseImage<T>::BaseImage(Shape size, uint number_of_channels)
 : _size_x(size.size_x), _size_y(size.size_y), _number_of_channels(number_of_channels)
{
	init(size.size_x, size.size_y, number_of_channels);
}


template <class T>
BaseImage<T>::BaseImage(Shape size, T default_value)
 : _size_x(size.size_x), _size_y(size.size_y), _number_of_channels(1)
{
	init(size.size_x, size.size_y, 1);
	fill_internal(default_value);
}


template <class T>
BaseImage<T>::BaseImage(Shape size, uint number_of_channels, T default_value)
 : _size_x(size.size_x), _size_y(size.size_y), _number_of_channels(number_of_channels)
{
	init(size.size_x, size.size_y, number_of_channels);
	fill_internal(default_value);
}


template <class T>
BaseImage<T>::BaseImage(const BaseImage<T> &source)
 : _size_x(source._size_x), _size_y(source._size_y), _number_of_channels(source._number_of_channels), _ref(source._ref), _data(source._data)
{
	if (_ref) {
		_ref->counter++;
	}
}


template <class T>
BaseImage<T>::~BaseImage()
{
	release();
}


template <class T>
uint BaseImage<T>::get_size_x() const
{
	return _size_x;
}


template <class T>
uint BaseImage<T>::get_size_y() const
{
	return _size_y;
}


template <class T>
uint BaseImage<T>::get_number_of_channels() const
{
	return _number_of_channels;
}


template <class T>
const T& BaseImage<T>::operator() (uint x, uint y) const
{
	return *(const T*)(_data + get_index(x, y, 0));
}


template <class T>
const T& BaseImage<T>::operator() (uint x, uint y, uint channel) const
{
	return *(const T*)(_data + get_index(x, y, channel));
}


template <class T>
const T& BaseImage<T>::operator() (Point p) const
{
	return *(const T*)(_data + get_index(p.x, p.y, 0));
}


template <class T>
const T& BaseImage<T>::operator() (Point p, uint channel) const
{
	return *(const T*)(_data + get_index(p.x, p.y, channel));
}


template <class T>
const T& BaseImage<T>::at(uint x, uint y) const
{
	if (x >= _size_x || y >= _size_y) {
		throw std::out_of_range();
	}

	return (const T*)(_data + get_index(x, y, 0));
}


template <class T>
const T& BaseImage<T>::at(uint x, uint y, uint channel) const
{
	if (x >= _size_x || y >= _size_y || channel >= _number_of_channels) {
		throw std::out_of_range();
	}

	return (const T*)(_data + get_index(x, y, channel));
}


template <class T>
const T& BaseImage<T>::at(Point p) const
{
	if (p.x >= _size_x || p.y >= _size_y) {
		throw std::out_of_range();
	}

	return (const T*)(_data + get_index(p.x, p.y, 0));
}


template <class T>
const T& BaseImage<T>::at(Point p, uint channel) const
{
	if (p.x >= _size_x || p.y >= _size_y || channel >= _number_of_channels) {
		throw std::out_of_range();
	}

	return (const T*)(_data + get_index(p.x, p.y, channel));
}


template <class T>
bool BaseImage<T>::try_get_value(uint x, uint y, T& value) const
{
	if (x >= this->_size_x || y >= this->_size_y || !this->_data) {
		return false;
	}

	value = this->_data[get_index(x, y, 0)];

	return true;
}


template <class T>
bool BaseImage<T>::try_get_value(uint x, uint y, uint channel, T& value) const
{
	if (x >= _size_x || y >= _size_y || channel >= _number_of_channels || !_data) {
		return false;
	}

	value = _data[get_index(x, y, channel)];

	return true;
}


template <class T>
bool BaseImage<T>::try_get_value(Point p, T& value) const
{
	if (p.x >= _size_x || p.y >= _size_y || !_data) {
		return false;
	}

	value = _data[get_index(p.x, p.y, 0)];

	return true;
}


template <class T>
bool BaseImage<T>::try_get_value(Point p, uint channel, T& value) const
{
	if (p.x >= _size_x || p.y >= _size_y || channel >= _number_of_channels || !_data) {
		return false;
	}

	value = _data[get_index(p.x, p.y, channel)];

	return true;
}


template <class T>
const T* BaseImage<T>::raw() const
{
	return this->_data;
}


/* Protected */

template <class T>
void BaseImage<T>::fill_internal(const T &value)
{
	std::fill_n(_data, _number_of_channels * _size_y * _size_x, value);
}


template <class T>
void BaseImage<T>::release() const
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
inline void BaseImage<T>::init(uint size_x, uint size_y, uint number_of_channels)
{
	_data = new T[size_x * size_y * number_of_channels];
	_ref = new __Ref();
	_ref->counter = 1;
}


template <class T>
inline uint BaseImage<T>::get_index(uint x, uint y, uint channel) const
{
	return _size_x * (_size_y * channel + y) + x;
}

