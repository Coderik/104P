/*
 * image.h
 *
 *  Created on: 26 марта 2014 г.
 *      Author: Coderik
 */

#ifndef IMAGE_H_
#define IMAGE_H_

#include <cstring>
#include <algorithm>
#include <stdexcept>
#include "base_image.h"

// TODO: handle casting to Sequence and back
typedef unsigned int uint;

using namespace std;

template <class T>
class ConstImage;

template <class T>
class Image : public BaseImage<T> {
friend class ConstImage<T>;
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
	Image(const Image<T> &source);		// ref++
	Image(const ConstImage<T> &source);	// deep copy
	virtual ~Image();

	Image<T>& operator= (Image<T> &other);
	Image<T>& operator= (ConstImage<T> &other);

	Image<T> clone() const;

	virtual T& operator() (uint x, uint y);
	virtual T& operator() (uint x, uint y, uint channel);
	virtual T& operator() (Point p);
	virtual T& operator() (Point p, uint channel);

	virtual T& at(uint x, uint y);
	virtual T& at(uint x, uint y, uint channel);
	virtual T& at(Point p);
	virtual T& at(Point p, uint channel);

	void fill(const T &value);

	T* raw();
};



template <class T = float>
class ConstImage : public BaseImage<T> {
friend class Image<T>;
public:
	ConstImage();
	ConstImage(uint size_x, uint size_y);
	ConstImage(uint size_x, uint size_y, uint number_of_channels);
	ConstImage(uint size_x, uint size_y, T default_value);
	ConstImage(uint size_x, uint size_y, uint number_of_channels, T default_value);
	ConstImage(Shape size);
	ConstImage(Shape size, uint number_of_channels);
	ConstImage(Shape size, T default_value);
	ConstImage(Shape size, uint number_of_channels, T default_value);
	ConstImage(const ConstImage<T> &source);	// ref++
	ConstImage(const Image<T> &source);			// ref++
	~ConstImage();

	ConstImage<T>& operator= (ConstImage<T> &other);
	ConstImage<T>& operator= (Image<T> &other);

	ConstImage<T> clone() const;
};

// NOTE: include implementation, because Image is a template
#include "../image.hpp"

#endif /* IMAGE_H_ */
