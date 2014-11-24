/*
 * mask_sequence.cpp
 *
 *  Created on: Nov 13, 2014
 *      Author: upf
 */

#include "mask_sequence.h"

/* MaskSequence */

MaskSequence::MaskSequence()
{

}


MaskSequence::MaskSequence(Shape size)
: _size(size.size_x, size.size_y, 0)
{
	_frames.reserve(size.size_t);
}


MaskSequence::MaskSequence(uint size_x, uint size_y)
: _size(size_x, size_y, 0)
{

}


MaskSequence::MaskSequence(uint size_x, uint size_y, uint size_t)
{
	_frames.reserve(size_t);

	for (uint i = 0; i < size_t; i++) {
		_frames.push_back(Mask(size_x, size_y));
	}
}


MaskSequence::MaskSequence(uint size_x, uint size_y, uint size_t, bool default_value)
{
	_frames.reserve(size_t);

	for (uint i = 0; i < size_t; i++) {
		_frames.push_back(Mask(size_x, size_y, default_value));
	}
}


MaskSequence::MaskSequence(const Mask &frame, uint size_t)
{
	_frames.reserve(size_t);
	_frames.push_back(frame);

	_size = frame.size();
	_size.size_t = 1;
}


MaskSequence::MaskSequence(const MaskFx &frame, uint size_t)
{
	_frames.reserve(size_t);
	_frames.push_back(frame);

	_size = frame.size();
	_size.size_t = 1;
}


MaskSequence::MaskSequence(const MaskSequence &other)	// copy constructor
{

	vector<Mask> aux(other._frames.begin(), other._frames.end());
	_frames.swap(aux);
	_size = other._size;
}


MaskSequence::MaskSequence(const MaskSequenceFx &other)	// copy constructor
{
	vector<Mask> aux(other._frames.begin(), other._frames.end());
	_frames.swap(aux);
	_size = other._size;


}


MaskSequence::~MaskSequence()
{

}


bool MaskSequence::add(const Mask &frame)	// use &&
{
	if (frame.size().size_x == _size.size_x && frame.size().size_y == _size.size_y) {
		_frames.push_back(frame);
		_size.size_t++;
		return true;
	}

	return false;
}


uint MaskSequence::size_x() const
{
	return _size.size_x;
}


uint MaskSequence::size_y() const
{
	return _size.size_y;
}


uint MaskSequence::size_t() const
{
	return _size.size_t;
}


Shape MaskSequence::size() const
{
	return _size;
}


MaskSequence::operator bool() const
{
	return _size.size_t != 0;
}


bool MaskSequence::is_empty() const
{
	return _size.size_t == 0;
}


bool& MaskSequence::operator() (uint x, uint y, uint t)
{
	return _frames[t](x, y);
}


bool& MaskSequence::operator() (const Point &p)
{
	return _frames[p.t](p);
}


/// Returns value without range checking.
const Mask& MaskSequence::operator[] (uint t) const
{
	return _frames[t];
}


Mask& MaskSequence::operator[] (uint t)
{
	return _frames[t];
}


/// Returns value with range checking.
/// Throws std::out_of_range exception, if out of range.
const Mask& MaskSequence::frame(uint t) const
{
	if (t >= _size.size_t) {
		throw std::out_of_range("t coordinate is out of range");
	}

	return _frames[t];
}


Mask& MaskSequence::frame(uint t)
{
	if (t >= _size.size_t) {
		throw std::out_of_range("t coordinate is out of range");
	}

	return _frames[t];
}


MaskSequence MaskSequence::clone() const
{
	MaskSequence cloned(_size.size_x, _size.size_y);

	for (uint t = 0; t < _size.size_t; t++) {
		cloned.add(_frames[t].clone());
	}

	return cloned;
}


void MaskSequence::invert()
{
	for (uint t = 0; t < _size.size_t; t++) {
		_frames[t].invert();
	}
}


MaskSequence::iterator MaskSequence::begin() const
{
	return iterator(this, first());
}


MaskSequence::iterator MaskSequence::end() const
{
	return iterator(this, Point(-1, -1, -1));
}


MaskSequence::iterator MaskSequence::rbegin() const
{
	return iterator(this, last(), true);
}


MaskSequence::iterator MaskSequence::rend() const
{
	return iterator(this, Point(-1, -1, -1), true);
}


Point MaskSequence::first() const
{
	for (uint t = 0; t < _size.size_t; t++) {
		for (uint y = 0; y < _size.size_y; y++) {
			for (uint x = 0; x < _size.size_x; x++) {
				if (_frames[t](x, y)) {
					return Point(x, y, t);
				}
			}
		}
	}

	return Point(-1, -1, -1);
}


Point MaskSequence::last() const
{
	for (uint t = _size.size_t - 1; t >= 0; t--) {
		for (uint y = _size.size_y - 1; y >= 0; y--) {
			for (uint x = _size.size_x - 1; x >= 0; x--) {
				if (_frames[t](x, y)) {
					return Point(x, y, t);
				}
			}
		}
	}

	return Point(-1, -1, -1);
}


Point MaskSequence::next(const Point &current) const
{
	int from_x = current.x + 1;
	int from_y = current.y;
	for (int t = current.t; t < _size.size_t; t++) {
		for (int y = from_y; y < _size.size_y; y++) {
			for (int x = from_x; x < _size.size_x; x++) {
				if (_frames[t](x, y)) {
					return Point(x, y, t);
				}
			}
			from_x = 0;
		}
		from_y = 0;
	}

	return Point(-1, -1, -1);
}


Point MaskSequence::prev(const Point &current) const
{
	int from_x = current.x - 1;
	int from_y = current.y;
	for (int t = current.t; t >= 0; t--) {
		for (int y = from_y; y >= 0; y--) {
			for (int x = from_x; x >= 0; x--) {
				if (_frames[t](x, y)) {
					return Point(x, y, t);
				}
			}
			from_x = _size.size_x - 1;
		}
		from_y = _size.size_y - 1;
	}

	return Point(-1, -1, -1);
}


/* MaskSequenceFx */

const MaskSequenceFx MaskSequenceFx::empty = MaskSequenceFx();

MaskSequenceFx::MaskSequenceFx()
{

}


MaskSequenceFx::MaskSequenceFx(Shape size)
: _size(size.size_x, size.size_y, 0)
{
	_frames.reserve(size.size_t);
}


MaskSequenceFx::MaskSequenceFx(uint size_x, uint size_y)
: _size(size_x, size_y, 0)
{

}


MaskSequenceFx::MaskSequenceFx(const MaskFx &frame, uint size_t)
{
	_frames.reserve(size_t);
	_frames.push_back(frame);

	_size = frame.size();
	_size.size_t = 1;
}


MaskSequenceFx::MaskSequenceFx(const Mask &frame, uint size_t)
{
	_frames.reserve(size_t);
	_frames.push_back(frame);

	_size = frame.size();
	_size.size_t = 1;
}


MaskSequenceFx::MaskSequenceFx(const MaskSequenceFx &other)	// copy constructor
{
	vector<MaskFx > aux(other._frames.begin(), other._frames.end());
	_frames.swap(aux);
	_size = other._size;
}


MaskSequenceFx::MaskSequenceFx(const MaskSequence &other)	// copy constructor
{
	vector<MaskFx > aux(other._frames.begin(), other._frames.end());
	_frames.swap(aux);
	_size = other._size;
}


MaskSequenceFx::~MaskSequenceFx()
{

}


bool MaskSequenceFx::add(const MaskFx &frame)	// use &&
{
	if (frame.size().size_x == _size.size_x && frame.size().size_y == _size.size_y) {
		_frames.push_back(frame);
		_size.size_t++;
		return true;
	}

	return false;
}


uint MaskSequenceFx::size_x() const
{
	return _size.size_x;
}


uint MaskSequenceFx::size_y() const
{
	return _size.size_y;
}


uint MaskSequenceFx::size_t() const
{
	return _size.size_t;
}


Shape MaskSequenceFx::size() const
{
	return _size;
}


MaskSequenceFx::operator bool() const
{
	return _size.size_t != 0;
}


bool MaskSequenceFx::is_empty() const
{
	return _size.size_t == 0;
}


const bool& MaskSequenceFx::operator() (uint x, uint y, uint t) const
{
	return _frames[t](x, y);
}


const bool& MaskSequenceFx::operator() (const Point &p) const
{
	return _frames[p.t](p);
}


/// Returns value without range checking.
const MaskFx& MaskSequenceFx::operator[] (uint t) const
{
	return (const MaskFx)_frames[t];
}


MaskFx& MaskSequenceFx::operator[] (uint t)
{
	return _frames[t];
}


/// Returns value with range checking.
/// Throws std::out_of_range exception, if out of range.
const MaskFx& MaskSequenceFx::frame(uint t) const
{
	if (t >= _size.size_t) {
		throw std::out_of_range("t coordinate is out of range");
	}

	return (const MaskFx)_frames[t];
}


MaskFx& MaskSequenceFx::frame(uint t)
{
	if (t >= _size.size_t) {
		throw std::out_of_range("t coordinate is out of range");
	}

	return _frames[t];
}


MaskSequenceFx MaskSequenceFx::clone() const
{
	MaskSequenceFx cloned(_size.size_x, _size.size_y);

	for (uint t = 0; t < _size.size_t; t++) {
		cloned.add(_frames[t].clone());
	}

	return cloned;
}


MaskSequenceFx::iterator MaskSequenceFx::begin() const
{
	return iterator(this, first());
}


MaskSequenceFx::iterator MaskSequenceFx::end() const
{
	return iterator(this, Point(-1, -1, -1));
}


MaskSequenceFx::iterator MaskSequenceFx::rbegin() const
{
	return iterator(this, last(), true);
}


MaskSequenceFx::iterator MaskSequenceFx::rend() const
{
	return iterator(this, Point(-1, -1, -1), true);
}


Point MaskSequenceFx::first() const
{
	for (uint t = 0; t < _size.size_t; t++) {
		for (uint y = 0; y < _size.size_y; y++) {
			for (uint x = 0; x < _size.size_x; x++) {
				if (_frames[t](x, y)) {
					return Point(x, y, t);
				}
			}
		}
	}

	return Point(-1, -1, -1);
}


Point MaskSequenceFx::last() const
{
	for (uint t = _size.size_t - 1; t >= 0; t--) {
		for (uint y = _size.size_y - 1; y >= 0; y--) {
			for (uint x = _size.size_x - 1; x >= 0; x--) {
				if (_frames[t](x, y)) {
					return Point(x, y, t);
				}
			}
		}
	}

	return Point(-1, -1, -1);
}


Point MaskSequenceFx::next(const Point &current) const
{
	int from_x = current.x + 1;
	int from_y = current.y;
	for (uint t = current.t; t < _size.size_t; t++) {
		for (uint y = from_y; y < _size.size_y; y++) {
			for (uint x = from_x; x < _size.size_x; x++) {
				if (_frames[t](x, y)) {
					return Point(x, y, t);
				}
			}
			from_x = 0;
		}
		from_y = 0;
	}

	return Point(-1, -1, -1);
}


Point MaskSequenceFx::prev(const Point &current) const
{
	int from_x = current.x - 1;
	int from_y = current.y;
	for (int t = current.t; t >= 0; t--) {
		for (int y = from_y; y >= 0; y--) {
			for (int x = from_x; x >= 0; x--) {
				if (_frames[t](x, y)) {
					return Point(x, y, t);
				}
			}
			from_x = _size.size_x - 1;
		}
		from_y = _size.size_y - 1;
	}

	return Point(-1, -1, -1);
}
