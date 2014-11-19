/*
 * sequence.h
 *
 *  Created on: Nov 11, 2014
 *      Author: upf
 */

#ifndef SEQUENCE_H_
#define SEQUENCE_H_

#include <vector>
#include "image.h"
#include "mask.h"

using namespace std;

// TODO: there is no big meaning to create *Fx directly. So maybe keep only copy constructors for them?

template<class T = float>
class SequenceFx;

template<class T = float>
class Sequence
{
friend class SequenceFx<T>;
public:
	Sequence();
	Sequence(Shape size);
	Sequence(Shape size, T default_value);
	Sequence(uint size_x, uint size_y);
	Sequence(uint size_x, uint size_y, uint size_t);
	Sequence(uint size_x, uint size_y, uint size_t, T default_value);
	Sequence(const Image<T> &frame, uint size_t = 1);
	Sequence(const ImageFx<T> &frame, uint size_t = 1);
	~Sequence();

	Sequence(const Sequence<T> &other);		// copy constructor
	Sequence(const SequenceFx<T> &other);	// copy constructor

	Sequence<T>& operator= (const Sequence<T> &other);
	Sequence<T>& operator= (const SequenceFx<T> &other);

	bool add(const Image<T> &frame);

	uint size_x() const;
	uint size_y() const;
	uint size_t() const;
	Shape size() const;

	/// Is current sequence not empty.
	operator bool() const;
	bool is_empty() const;

	// NOTE: added for convenience
	const T& operator() (uint x, uint y, uint t) const;
	const T& operator() (const Point &p) const;
	T& operator() (uint x, uint y, uint t);
	T& operator() (const Point &p);

	/// Returns value without range checking.
	const Image<T>& operator[] (uint t) const;
	Image<T>& operator[] (uint t);

	Sequence<T> clone() const;

	/// Returns value with range checking.
	/// Throws std::out_of_range exception, if out of range.
	const Image<T>& frame(uint t) const;
	Image<T>& frame(uint t);

private:
	vector<Image<T> > _frames;
	Shape _size;
};


template<class T>
class SequenceFx
{
friend class Sequence<T>;
public:
	SequenceFx();
	SequenceFx(Shape size);
	SequenceFx(uint size_x, uint size_y);
	SequenceFx(const ImageFx<T> &frame, uint size_t = 1);
	SequenceFx(const Image<T> &frame, uint size_t = 1);
	~SequenceFx();

	SequenceFx(const SequenceFx<T> &other);	// copy constructor
	SequenceFx(const Sequence<T> &other);	// copy constructor

	SequenceFx<T>& operator= (const SequenceFx<T> &other);
	SequenceFx<T>& operator= (const Sequence<T> &other);

	bool add(const ImageFx<T> &frame);	// TODO: remove this eventually

	uint size_x() const;
	uint size_y() const;
	uint size_t() const;
	Shape size() const;

	operator bool() const;
	bool is_empty() const;

	// NOTE: added for convenience
	const T& operator() (uint x, uint y, uint t) const;
	const T& operator() (const Point &p) const;

	/// Returns value without range checking.
	const ImageFx<T>& operator[] (uint t) const;
	ImageFx<T>& operator[] (uint t);

	SequenceFx<T> clone() const;

	/// Returns value with range checking.
	/// Throws std::out_of_range exception, if out of range.
	const ImageFx<T>& frame(uint t) const;
	ImageFx<T>& frame(uint t);

private:
	vector<ImageFx<T> > _frames;
	Shape _size;
};


#include "../sequence.hpp"

#endif /* SEQUENCE_H_ */
