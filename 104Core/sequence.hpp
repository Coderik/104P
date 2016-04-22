/*
 * sequence.hpp
 *
 *  Created on: Nov 11, 2014
 *      Author: upf
 */

/* Sequence */

template<class T>
Sequence<T>::Sequence()
{

}


template<class T>
Sequence<T>::Sequence(Shape size)
: _size(size)
{
	_frames.reserve(size.size_t);

	for (int i = 0; i < size.size_t; i++) {
		_frames.push_back(Image<T>(size));
	}
}


template<class T>
Sequence<T>::Sequence(Shape size, T default_value)
: _size(size)
{
	_frames.reserve(size.size_t);

	for (int i = 0; i < size.size_t; i++) {
		_frames.push_back(Image<T>(size, default_value));
	}
}


template<class T>
Sequence<T>::Sequence(uint size_x, uint size_y)
: _size(size_x, size_y, 0)
{

}


template<class T>
Sequence<T>::Sequence(uint size_x, uint size_y, uint size_t)
: _size(size_x, size_y, size_t)
{
	_frames.reserve(size_t);

	for (uint i = 0; i < size_t; i++) {
		_frames.push_back(Image<T>(size_x, size_y));
	}
}


template<class T>
Sequence<T>::Sequence(uint size_x, uint size_y, uint size_t, T default_value)
: _size(size_x, size_y, size_t)
{
	_frames.reserve(size_t);

	for (int i = 0; i < size_t; i++) {
		_frames.push_back(Image<T>(_size, default_value));
	}
}


template<class T>
Sequence<T>::Sequence(const Image<T> &frame, uint size_t)
{
	_frames.reserve(size_t);
	_frames.push_back(frame);

	_size = frame.size();
	_size.size_t = 1;
}


template<class T>
Sequence<T>::Sequence(const ImageFx<T> &frame, uint size_t)
{
	_frames.reserve(size_t);
	_frames.push_back(frame);

	_size = frame.size();
	_size.size_t = 1;
}


template<class T>
Sequence<T>::Sequence(const Sequence<T> &other)	// copy constructor
{

	vector<Image<T> > aux(other._frames.begin(), other._frames.end());
	_frames.swap(aux);
	_size = other._size;
}


template<class T>
Sequence<T>::Sequence(const SequenceFx<T> &other)	// copy constructor
{
	vector<Image<T> > aux(other._frames.begin(), other._frames.end());
	_frames.swap(aux);
	_size = other._size;
}


template <class T>
Sequence<T>& Sequence<T>::operator= (const Sequence<T> &other)
{
	// check for self-assignment
	if(this == &other) {
		return *this;
	}

	vector<Image<T> > aux(other._frames.begin(), other._frames.end());
	_frames.swap(aux);
	_size = other._size;

	return *this;
}


template <class T>
Sequence<T>& Sequence<T>::operator= (const SequenceFx<T> &other)
{
	vector<Image<T> > aux(other._frames.begin(), other._frames.end());
	_frames.swap(aux);
	_size = other._size;

	return *this;
}


template<class T>
Sequence<T>::~Sequence()
{

}


template<class T>
bool Sequence<T>::add(const Image<T> &frame)	// use &&
{
	if (frame.size().size_x == _size.size_x && frame.size().size_y == _size.size_y) {
		_frames.push_back(frame);
		_size.size_t++;
		return true;
	}

	return false;
}


template<class T>
uint Sequence<T>::size_x() const
{
	return _size.size_x;
}


template<class T>
uint Sequence<T>::size_y() const
{
	return _size.size_y;
}


template<class T>
uint Sequence<T>::size_t() const
{
	return _size.size_t;
}


template<class T>
Shape Sequence<T>::size() const
{
	return _size;
}


template<class T>
Sequence<T>::operator bool() const
{
	return _size.size_t != 0;
}


template<class T>
bool Sequence<T>::is_empty() const
{
	return _size.size_t == 0;
}


template<class T>
const T& Sequence<T>::operator() (uint x, uint y, uint t) const
{
	return _frames[t](x, y);
}


template<class T>
const T& Sequence<T>::operator() (const Point &p) const
{
	return _frames[p.t](p);
}


template<class T>
T& Sequence<T>::operator() (uint x, uint y, uint t)
{
	return _frames[t](x, y);
}


template<class T>
T& Sequence<T>::operator() (const Point &p)
{
	return _frames[p.t](p);
}


template<class T>
const T& Sequence<T>::operator() (uint x, uint y, uint t, uint channel) const
{
	return _frames[t](x, y, channel);
}


template<class T>
const T& Sequence<T>::operator() (const Point &p, uint channel) const
{
	return _frames[p.t](p, channel);
}


template<class T>
T& Sequence<T>::operator() (uint x, uint y, uint t, uint channel)
{
	return _frames[t](x, y, channel);
}


template<class T>
T& Sequence<T>::operator() (const Point &p, uint channel)
{
	return _frames[p.t](p, channel);
}


template<class T>
bool Sequence<T>::try_get_value(uint x, uint y, uint t, T& value) const
{
	if (t >= _frames.size()) {
		return false;
	}

	return _frames[t].try_get_value(x, y, value);
}


template<class T>
bool Sequence<T>::try_get_value(const Point &p, T& value) const
{
	if (p.t >= _frames.size()) {
		return false;
	}

	return _frames[p.t].try_get_value(p, value);
}


/// Returns value without range checking.
template<class T>
const Image<T>& Sequence<T>::operator[] (uint t) const
{
	return _frames[t];
}


template<class T>
Image<T>& Sequence<T>::operator[] (uint t)
{
	return _frames[t];
}


template<class T>
Sequence<T> Sequence<T>::clone() const
{
	Sequence<T> cloned(_size.size_x, _size.size_y);

	for (uint t = 0; t < _size.size_t; t++) {
		cloned.add(_frames[t].clone());
	}

	return cloned;
}


/// Returns value with range checking.
/// Throws std::out_of_range exception, if out of range.
template<class T>
const Image<T>& Sequence<T>::frame(uint t) const
{
	if (t >= _size.size_t) {
		throw std::out_of_range("t coordinate is out of range");
	}

	return (const Image<T>)_frames[t];
}


template<class T>
Image<T>& Sequence<T>::frame(uint t)
{
	if (t >= _size.size_t) {
		throw std::out_of_range("t coordinate is out of range");
	}

	return _frames[t];
}




/* SequenceFx */

template<class T>
SequenceFx<T>::SequenceFx()
{

}


template<class T>
SequenceFx<T>::SequenceFx(Shape size)
: _size(size.size_x, size.size_y, 0)
{
	_frames.reserve(size.size_t);
}


template<class T>
SequenceFx<T>::SequenceFx(uint size_x, uint size_y)
: _size(size_x, size_y, 0)
{

}


template<class T>
SequenceFx<T>::SequenceFx(const ImageFx<T> &frame, uint size_t)
{
	_frames.reserve(size_t);
	_frames.push_back(frame);

	_size = frame.size();
	_size.size_t = 1;
}


template<class T>
SequenceFx<T>::SequenceFx(const Image<T> &frame, uint size_t)
{
	_frames.reserve(size_t);
	_frames.push_back(frame);

	_size = frame.size();
	_size.size_t = 1;
}


template<class T>
SequenceFx<T>::SequenceFx(const SequenceFx<T> &other)	// copy constructor
{
	vector<ImageFx<T> > aux(other._frames.begin(), other._frames.end());
	_frames.swap(aux);
	_size = other._size;
}


template<class T>
SequenceFx<T>::SequenceFx(const Sequence<T> &other)	// copy constructor
{
	vector<ImageFx<T> > aux(other._frames.begin(), other._frames.end());
	_frames.swap(aux);
	_size = other._size;
}


template <class T>
SequenceFx<T>& SequenceFx<T>::operator= (const SequenceFx<T> &other)
{
	// check for self-assignment
	if(this == &other) {
		return *this;
	}

	vector<ImageFx<T> > aux(other._frames.begin(), other._frames.end());
	_frames.swap(aux);
	_size = other._size;

	return *this;
}


template <class T>
SequenceFx<T>& SequenceFx<T>::operator= (const Sequence<T> &other)
{
	vector<ImageFx<T> > aux(other._frames.begin(), other._frames.end());
	_frames.swap(aux);
	_size = other._size;

	return *this;
}


template<class T>
SequenceFx<T>::~SequenceFx()
{

}


template<class T>
bool SequenceFx<T>::add(const ImageFx<T> &frame)	// use &&
{
	if (frame.size().size_x == _size.size_x && frame.size().size_y == _size.size_y) {
		_frames.push_back(frame);
		_size.size_t++;
		return true;
	}

	return false;
}


template<class T>
uint SequenceFx<T>::size_x() const
{
	return _size.size_x;
}


template<class T>
uint SequenceFx<T>::size_y() const
{
	return _size.size_y;
}


template<class T>
uint SequenceFx<T>::size_t() const
{
	return _size.size_t;
}


template<class T>
Shape SequenceFx<T>::size() const
{
	return _size;
}


template<class T>
SequenceFx<T>::operator bool() const
{
	return _size.size_t != 0;
}


template<class T>
bool SequenceFx<T>::is_empty() const
{
	return _size.size_t == 0;
}


template<class T>
const T& SequenceFx<T>::operator() (uint x, uint y, uint t) const
{
	return _frames[t](x, y);
}


template<class T>
const T& SequenceFx<T>::operator() (const Point &p) const
{
	return _frames[p.t](p);
}


template<class T>
bool SequenceFx<T>::try_get_value(uint x, uint y, uint t, T& value) const
{
	if (t >= _frames.size()) {
		return false;
	}

	return _frames[t].try_get_value(x, y, value);
}


template<class T>
bool SequenceFx<T>::try_get_value(const Point &p, T& value) const
{
	if (p.t >= _frames.size()) {
		return false;
	}

	return _frames[p.t].try_get_value(p, value);
}


/// Returns value without range checking.
template<class T>
const ImageFx<T>& SequenceFx<T>::operator[] (uint t) const
{
	return _frames[t];
}


template<class T>
ImageFx<T>& SequenceFx<T>::operator[] (uint t)
{
	return _frames[t];
}


template<class T>
SequenceFx<T> SequenceFx<T>::clone() const
{
	SequenceFx<T> cloned(_size.size_x, _size.size_y);

	for (uint t = 0; t < _size.size_t; t++) {
		cloned.add(_frames[t].clone());
	}

	return cloned;
}


/// Returns value with range checking.
/// Throws std::out_of_range exception, if out of range.
template<class T>
const ImageFx<T>& SequenceFx<T>::frame(uint t) const
{
	if (t >= _size.size_t) {
		throw std::out_of_range("t coordinate is out of range");
	}

	return _frames[t];
}


template<class T>
ImageFx<T>& SequenceFx<T>::frame(uint t)
{
	if (t >= _size.size_t) {
		throw std::out_of_range("t coordinate is out of range");
	}

	return _frames[t];
}
