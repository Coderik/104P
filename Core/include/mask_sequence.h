/*
 * mask_sequence.h
 *
 *  Created on: Nov 13, 2014
 *      Author: upf
 */

#ifndef MASK_SEQUENCE_H_
#define MASK_SEQUENCE_H_

#include <vector>
#include "mask.h"

using namespace std;

class MaskSequenceFx;

class MaskSequence
{
friend class MaskSequenceFx;
public:
	MaskSequence();
	MaskSequence(Shape size);
	MaskSequence(uint size_x, uint size_y);
	MaskSequence(uint size_x, uint size_y, uint size_t);
	MaskSequence(const Mask &frame, uint size_t = 1);
	MaskSequence(const MaskFx &frame, uint size_t = 1);
	~MaskSequence();

	MaskSequence(const MaskSequence &other);	// copy constructor
	MaskSequence(const MaskSequenceFx &other);	// copy constructor

	bool add(const Mask &frame);

	uint size_x() const;
	uint size_y() const;
	uint size_t() const;
	Shape size() const;

	operator bool() const;
	bool is_empty() const;

	/// Returns value without range checking.
	const Mask& operator[] (uint t) const;
	Mask& operator[] (uint t);

	/// Returns value with range checking.
	/// Throws std::out_of_range exception, if out of range.
	const Mask& frame(uint t) const;
	Mask& frame(uint t);

private:
	vector<Mask > _frames;
	Shape _size;
};



class MaskSequenceFx
{
friend class MaskSequence;
public:
	MaskSequenceFx();
	MaskSequenceFx(Shape size);
	MaskSequenceFx(uint size_x, uint size_y);
	MaskSequenceFx(const MaskFx &frame, uint size_t = 1);
	MaskSequenceFx(const Mask &frame, uint size_t = 1);
	~MaskSequenceFx();

	MaskSequenceFx(const MaskSequenceFx &other);	// copy constructor
	MaskSequenceFx(const MaskSequence &other);	// copy constructor

	bool add(const MaskFx &frame);

	uint size_x() const;
	uint size_y() const;
	uint size_t() const;
	Shape size() const;

	operator bool() const;
	bool is_empty() const;

	/// Returns value without range checking.
	const MaskFx& operator[] (uint t) const;
	MaskFx& operator[] (uint t);

	/// Returns value with range checking.
	/// Throws std::out_of_range exception, if out of range.
	const MaskFx& frame(uint t) const;
	MaskFx& frame(uint t);

private:
	vector<MaskFx > _frames;
	Shape _size;
};

#endif /* MASK_SEQUENCE_H_ */
