/*
 * base_image.h
 *
 *  Created on: 31 марта 2014 г.
 *      Author: Coderik
 */

#ifndef BASE_IMAGE_H_
#define BASE_IMAGE_H_

#include "point.h"
#include "shape.h"

typedef unsigned int uint;

using namespace std;

template <class T = float>
class BaseImage {
public:
	BaseImage();
	BaseImage(uint size_x, uint size_y);
	BaseImage(uint size_x, uint size_y, uint number_of_channels);
	BaseImage(uint size_x, uint size_y, T default_value);
	BaseImage(uint size_x, uint size_y, uint number_of_channels, T default_value);
	BaseImage(Shape size);
	BaseImage(Shape size, uint number_of_channels);
	BaseImage(Shape size, T default_value);
	BaseImage(Shape size, uint number_of_channels, T default_value);
	BaseImage(const BaseImage<T> &source);
	~BaseImage();

	uint get_size_x() const;
	uint get_size_y() const;
	uint get_number_of_channels() const;

	const T& operator() (uint x, uint y) const;
	const T& operator() (uint x, uint y, uint channel) const;
	const T& operator() (Point p) const;
	const T& operator() (Point p, uint channel) const;

	const T& at(uint x, uint y) const;
	const T& at(uint x, uint y, uint channel) const;
	const T& at(Point p) const;
	const T& at(Point p, uint channel) const;

	bool try_get_value(uint x, uint y, T& value) const;
	bool try_get_value(uint x, uint y, uint channel, T& value) const;
	bool try_get_value(Point p, T& value) const;
	bool try_get_value(Point p, uint channel, T& value) const;

	const T* raw() const;
protected:
	struct __Ref {
		int counter;
	};

	uint _size_x, _size_y;
	uint _number_of_channels;
	T* _data;
	__Ref *_ref;

	void fill_internal(const T &value);
	void release() const;
	inline void init(uint size_x, uint size_y, uint number_of_channels);
	inline uint get_index(uint x, uint y, uint channel) const;
};


// NOTE: include implementation, because Image is a template
#include "../base_image.hpp"


#endif /* BASE_IMAGE_H_ */
