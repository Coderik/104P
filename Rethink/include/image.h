/*
 * image.h
 *
 *  Created on: 26 марта 2014 г.
 *      Author: Coderik
 */

#ifndef IMAGE_H_
#define IMAGE_H_

#include <algorithm>
#include <stdexcept>
#include "point.h"
#include "shape.h"

// TODO: handle casting to Sequence and back
typedef unsigned int uint;

template <class T = float>
class Image {
public:
	Image();
	Image(uint size_x, uint size_y);
	Image(uint size_x, uint size_y, uint number_of_channels);
	Image(uint size_x, uint size_y, T default_value);
	Image(uint size_x, uint size_y, uint number_of_channels, T default_value);
	Image(Shape size);
	Image(Shape size, uint number_of_channels);
	Image(Shape size, T default_value);
	Image(Shape size, uint number_of_channels, T default_value);
	Image(const Image<T> &source);
	~Image();

	const Image<T>& operator= (const Image<T> &other);

	Image<T> clone() const;

	uint get_size_x() const;
	uint get_size_y() const;
	uint get_number_of_channels() const;

	const T& operator() (uint x, uint y) const;
	const T& operator() (uint x, uint y, uint channel) const;
	const T& operator() (Point p) const;
	const T& operator() (Point p, uint channel) const;
	T& operator() (uint x, uint y);
	T& operator() (uint x, uint y, uint channel);
	T& operator() (Point p);
	T& operator() (Point p, uint channel);

	const T& at(uint x, uint y) const;
	const T& at(uint x, uint y, uint channel) const;
	const T& at(Point p) const;
	const T& at(Point p, uint channel) const;
	T& at(uint x, uint y);
	T& at(uint x, uint y, uint channel);
	T& at(Point p);
	T& at(Point p, uint channel);

	bool try_get_value(uint x, uint y, T& value) const;
	bool try_get_value(uint x, uint y, uint channel, T& value) const;
	bool try_get_value(Point p, T& value) const;
	bool try_get_value(Point p, uint channel, T& value) const;

	void fill(const T &value);

	const T* raw() const;
	T* raw();
private:
	struct __Ref {
		int counter;
	};

	uint _size_x, _size_y;
	uint _number_of_channels;
	T* _data;
	__Ref *_ref;

	void release();
	inline void init(uint size_x, uint size_y, uint number_of_channels);
	inline uint get_index(uint x, uint y, uint channel) const;
};

// NOTE: include implementation, because Image is a template
#include "../image.hpp"

#endif /* IMAGE_H_ */
