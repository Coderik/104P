/*
 * image.hpp
 *
 *  Created on: 27 марта 2014 г.
 *      Author: Coderik
 */

#include "image.h"


/* ==================== Image ==================== */


template <class T>
Image<T>::Image()
 : BaseImage<T>()
{
}


template <class T>
Image<T>::Image(uint size_x, uint size_y)
 : BaseImage<T>(size_x, size_y)
{
}


template <class T>
Image<T>::Image(uint size_x, uint size_y, uint number_of_channels)
 : BaseImage<T>(size_x, size_y, number_of_channels)
{
}


template <class T>
Image<T>::Image(uint size_x, uint size_y, T default_value)
 : BaseImage<T>(size_x, size_y, default_value)
{
}


template <class T>
Image<T>::Image(uint size_x, uint size_y, uint number_of_channels, T default_value)
 : BaseImage<T>(size_x, size_y, number_of_channels, default_value)
{
}


template <class T>
Image<T>::Image(Shape size)
 : BaseImage<T>(size)
{
}


template <class T>
Image<T>::Image(Shape size, uint number_of_channels)
 : BaseImage<T>(size, number_of_channels)
{
}


template <class T>
Image<T>::Image(Shape size, T default_value)
 : BaseImage<T>(size, default_value)
{
}


template <class T>
Image<T>::Image(Shape size, uint number_of_channels, T default_value)
 : BaseImage<T>(size, number_of_channels, default_value)
{
}


template <class T>
Image<T>::Image(const Image<T> &source)
 : BaseImage<T>(source)
{
}


template <class T>
Image<T>::Image(const ConstImage<T> &source)
{
	this->_size_x = source._size_x;
	this->_size_y = source._size_y;
	this->_number_of_channels = source._number_of_channels;
	Image<T>::init(source._size_x, source._size_y, source._number_of_channels);
	memcpy(this->_data, source._data,  source._number_of_channels * source._size_y * source._size_x * sizeof(T));
}


template <class T>
Image<T>::~Image()
{
}


template <class T>
Image<T>& Image<T>::operator= (Image<T> &other)
{
	// check for self-assignment
	if(this == &other) {
		return *this;
	}

	// finish all deals with the previous data
	Image<T>::release();

	// increase counter
	if (other._ref) {
		other._ref->counter++;
	}

	// assign new data
	this->_ref = other._ref;
	this->_size_x = other._size_x;
	this->_size_y = other._size_y;
	this->_number_of_channels = other._number_of_channels;
	this->_data = other._data;

	return *this;
}


template <class T>
Image<T>& Image<T>::operator= (ConstImage<T> &other)
{
	// finish all deals with the previous data
	Image<T>::release();

	this->_size_x = other._size_x;
	this->_size_y = other._size_y;
	this->_number_of_channels = other._number_of_channels;
	Image<T>::init(other._size_x, other._size_y, other._number_of_channels);
	memcpy(this->_data, other._data,  other._number_of_channels * other._size_y * other._size_x * sizeof(T));

	return *this;
}


template <class T>
Image<T> Image<T>::clone() const
{
	Image<T> clone;
	clone._size_x = this->_size_x;
	clone._size_y = this->_size_y;
	clone._number_of_channels = this->_number_of_channels;
	clone.init(this->_size_x, this->_size_y, this->_number_of_channels);
	memcpy(clone._data, this->_data,  this->_number_of_channels * this->_size_y * this->_size_x * sizeof(T));

	return clone;
}


template <class T>
T& Image<T>::operator() (uint x, uint y)
{
	return this->_data[Image<T>::get_index(x, y, 0)];
}


template <class T>
T& Image<T>::operator() (uint x, uint y, uint channel)
{
	return this->_data[Image<T>::get_index(x, y, channel)];
}


template <class T>
T& Image<T>::operator() (Point p)
{
	return this->_data[Image<T>::get_index(p.x, p.y, 0)];
}


template <class T>
T& Image<T>::operator() (Point p, uint channel)
{
	return this->_data[Image<T>::get_index(p.x, p.y, channel)];
}


template <class T>
T& Image<T>::at(uint x, uint y)
{
	if (x >= this->_size_x || y >= this->_size_y) {
		throw std::out_of_range("");					// TODO: add exception messages everywhere
	}

	return this->_data[Image<T>::get_index(x, y, 0)];
}


template <class T>
T& Image<T>::at(uint x, uint y, uint channel)
{
	if (x >= this->_size_x || y >= this->_size_y || channel >= this->_number_of_channels) {
		throw std::out_of_range("");
	}

	return this->_data[Image<T>::get_index(x, y, channel)];
}


template <class T>
T& Image<T>::at(Point p)
{
	if (p.x < 0 || p.x >= this->_size_x || p.y < 0 || p.y >= this->_size_y) {
		throw std::out_of_range("");
	}

	return this->_data[Image<T>::get_index(p.x, p.y, 0)];
}


template <class T>
T& Image<T>::at(Point p, uint channel)
{
	if (p.x < 0 || p.x >= this->_size_x || p.y < 0 || p.y >= this->_size_y || channel >= this->_number_of_channels) {
		throw std::out_of_range("");
	}

	return this->_data[Image<T>::get_index(p.x, p.y, channel)];
}


template <class T>
void Image<T>::fill(const T &value)
{
	Image<T>::fill_internal(value);
}


template <class T>
T* Image<T>::raw()
{
	return this->_data;
}


/* ==================== ConstImage ==================== */


template <class T>
ConstImage<T>::ConstImage()
 : BaseImage<T>()
{
}


template <class T>
ConstImage<T>::ConstImage(uint size_x, uint size_y)
 : BaseImage<T>(size_x, size_y)
{
}


template <class T>
ConstImage<T>::ConstImage(uint size_x, uint size_y, uint number_of_channels)
 : BaseImage<T>(size_x, size_y, number_of_channels)
{
}


template <class T>
ConstImage<T>::ConstImage(uint size_x, uint size_y, T default_value)
 : BaseImage<T>(size_x, size_y, default_value)
{
}


template <class T>
ConstImage<T>::ConstImage(uint size_x, uint size_y, uint number_of_channels, T default_value)
 : BaseImage<T>(size_x, size_y, number_of_channels, default_value)
{
}


template <class T>
ConstImage<T>::ConstImage(Shape size)
 : BaseImage<T>(size)
{
}


template <class T>
ConstImage<T>::ConstImage(Shape size, uint number_of_channels)
 : BaseImage<T>(size, number_of_channels)
{
}


template <class T>
ConstImage<T>::ConstImage(Shape size, T default_value)
 : BaseImage<T>(size, default_value)
{
}


template <class T>
ConstImage<T>::ConstImage(Shape size, uint number_of_channels, T default_value)
 : BaseImage<T>(size, number_of_channels, default_value)
{
}


template <class T>
ConstImage<T>::ConstImage(const ConstImage<T> &source)
 : BaseImage<T>(source)
{
}


template <class T>
ConstImage<T>::ConstImage(const Image<T> &source)
 : BaseImage<T>(source)
{
}


template <class T>
ConstImage<T>::~ConstImage()
{
}


template <class T>
ConstImage<T>& ConstImage<T>::operator= (ConstImage<T> &other)
{
	// check for self-assignment
	if(this == &other) {
		return *this;
	}

	// finish all deals with the previous data
	ConstImage<T>::release();

	// increase counter
	if (other._ref) {
		other._ref->counter++;
	}

	// assign new data
	this->_ref = other._ref;
	this->_size_x = other._size_x;
	this->_size_y = other._size_y;
	this->_number_of_channels = other._number_of_channels;
	this->_data = other._data;

	return *this;
}


template <class T>
ConstImage<T>& ConstImage<T>::operator= (Image<T> &other)
{
	// finish all deals with the previous data
	ConstImage<T>::release();

	// increase counter
	if (other._ref) {
		other._ref->counter++;
	}

	// assign new data
	this->_ref = other._ref;
	this->_size_x = other._size_x;
	this->_size_y = other._size_y;
	this->_number_of_channels = other._number_of_channels;
	this->_data = other._data;

	return *this;
}


template <class T>
ConstImage<T> ConstImage<T>::clone() const
{
	ConstImage<T> clone;
	clone._size_x = this->_size_x;
	clone._size_y = this->_size_y;
	clone._number_of_channels = this->_number_of_channels;
	clone.init(this->_size_x, this->_size_y, this->_number_of_channels);
	memcpy(clone._data, this->_data,  this->_number_of_channels * this->_size_y * this->_size_x * sizeof(T));

	return clone;
}
