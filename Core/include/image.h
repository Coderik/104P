/*
 * image.h
 *
 *  Created on: March 26, 2014
 *      Author: Vadim Fedorov
 */

#ifndef IMAGE_H_
#define IMAGE_H_

#include <cstring>
#include <algorithm>
#include <stdexcept>
#include "point.h"
#include "shape.h"

using namespace std;

template <class T>
class Image;	// forward declaration

/**
 * Container for a 2d image with point type T. Manages memory internally by
 * references counting. Constructing from the same type and assignment of
 * a value of the same type lead to a data sharing. Method clone() should be
 * used for explicit deep copy invocation.
 *
 * @note By design class provides no capabilities for changing its data,
 * therefore, 'Fixed' here should be considered as 'Immutable'.
 */
template <class T = float>
class ImageFx
{
friend class Image<T>;
public:
	ImageFx();
	ImageFx(uint size_x, uint size_y);
	ImageFx(uint size_x, uint size_y, uint number_of_channels);
	ImageFx(uint size_x, uint size_y, T default_value);
	ImageFx(uint size_x, uint size_y, uint number_of_channels, T default_value);
	ImageFx(Shape size);
	ImageFx(Shape size, uint number_of_channels);
	ImageFx(Shape size, T default_value);
	ImageFx(Shape size, uint number_of_channels, T default_value);
	ImageFx(const ImageFx<T> &source);				// without data copying, ref++
	ImageFx(const Image<T> &source);				// without data copying, ref++
	virtual ~ImageFx();

	ImageFx<T>& operator= (const ImageFx<T> &other);		// without data copying, ref++
	ImageFx<T>& operator= (const Image<T> &other);			// without data copying, ref++

	/// Is current image not empty.
	operator bool() const;
	bool is_empty() const;

	uint size_x() const;
	uint size_y() const;
	Shape size() const;
	uint number_of_channels() const;

	/// Returns read-only value without range checking.
	const T& operator() (uint x, uint y) const;
	const T& operator() (uint x, uint y, uint channel) const;
	const T& operator() (const Point &p) const;
	const T& operator() (const Point &p, uint channel) const;

	/// Returns read-only value with range checking.
	/// Throws std::out_of_range exception, if out of range.
	const T& at(uint x, uint y) const;
	const T& at(uint x, uint y, uint channel) const;
	const T& at(const Point &p) const;
	const T& at(const Point &p, uint channel) const;

	/// Checks if all indexes are in range and modifies the value parameter.
	bool try_get_value(uint x, uint y, T& value) const;
	bool try_get_value(uint x, uint y, uint channel, T& value) const;
	bool try_get_value(const Point &p, T& value) const;
	bool try_get_value(const Point &p, uint channel, T& value) const;

	/// Returns pointer to internal data.
	const T* raw() const;
	const uint raw_length() const;

	/// Invokes deep copy.
	ImageFx<T> clone() const;

protected:
	struct __Ref {
		int counter;
	};

	uint _size_x, _size_y;
	uint _number_of_channels;
	T* _data;
	__Ref *_ref;

	inline void init(uint size_x, uint size_y, uint number_of_channels);
	void fill_internal(const T &value);

	void release() const;
	virtual void destroy() const;

	inline uint index(uint x, uint y, uint channel) const;
};


/**
 * Container for a 2d image with point type T. Manages memory internally by
 * references counting. Constructing from the same type and assignment of
 * a value of the same type lead to a data sharing. Method clone() should be
 * used for explicit deep copy invocation.
 *
 * @note Extends the FixedImage<T> class with the data modification capabilities.
 */
template <class T>
class Image : public ImageFx<T>
{
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
	Image(const Image<T> &source);				// without data copying, ref++
	Image(const ImageFx<T> &source);			// deep copy
	virtual ~Image();

	Image<T>& operator= (const Image<T> &other);			// without data copying, ref++
	Image<T>& operator= (const ImageFx<T> &other);			// deep copy

	// prevent hiding of const versions of these methods
	using ImageFx<T>::operator();
	using ImageFx<T>::at;
	using ImageFx<T>::raw;

	/// Returns a reference to the element without range checking.
	T& operator() (uint x, uint y);
	T& operator() (uint x, uint y, uint channel);
	T& operator() (const Point &p);
	T& operator() (const Point &p, uint channel);

	/// Returns a reference to the element with range checking.
	/// Throws std::out_of_range exception, if out of range.
	T& at(uint x, uint y);
	T& at(uint x, uint y, uint channel);
	T& at(const Point &p);
	T& at(const Point &p, uint channel);

	/// Assigns a given value to all elements.
	void fill(const T &value);

	/// Returns pointer to internal data.
	T* raw();

	/// Invokes deep copy.
	Image<T> clone() const;

protected:
	virtual void destroy() const;
};

// NOTE: include implementation, because Image is a template
#include "../image.hpp"

#endif /* IMAGE_H_ */
