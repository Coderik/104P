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
		_frames.push_back(Mask(size));
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




/* MaskSequenceFx */

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



