/*
 * Sequence.cpp
 *
 *  Created on: Nov 26, 2012
 *      Author: Vadim Fedorov
 */

#include "headers/sequence.h"

Sequence::Sequence(int x_size, int y_size, int t_size, float value)
{
	_x_size = x_size;
	_y_size = y_size;
	_t_size = t_size;

	_frames = vector<Image* >(_t_size);

	fill(value);
}


Sequence::Sequence(int x_size,int y_size,int t_size)
{
	_x_size = x_size;
	_y_size = y_size;
	_t_size = t_size;

	_frames = vector<Image* >(_t_size);
}


Sequence::Sequence(int x_size,int y_size)
{
	_x_size = x_size;
	_y_size = y_size;
	_t_size = 0;

	_frames = vector<Image* >();
}


Sequence::Sequence(Image *first_frame)
{
	_x_size = first_frame->GetXSize();
	_y_size = first_frame->GetYSize();
	_t_size = 1;

	_frames = vector<Image* >(1, first_frame);
}

Sequence::~Sequence()
{
	_frames.clear();
}

int Sequence::GetXSize()
{
	return _x_size;
}

int Sequence::GetYSize()
{
	return _y_size;
}

int Sequence::GetTSize()
{
	return _t_size;
}


double Sequence::GetPixelValue(int x, int y, int t)
{
	if (t < 0 || t > _t_size || !_frames[t])
		return -1;

	return _frames[t]->GetPixelValue(x, y);
}


void Sequence::SetPixelValue(int x, int y, int t, double value)
{
	if (t < 0 || t > _t_size)
		return;

	if (!_frames[t]) {
		_frames[t] = new Image(_x_size, _y_size);
	}

	_frames[t]->SetPixelValue(x, y, value);
}


void Sequence::fill(float value)
{
	for (int t = 0; t < _t_size; t++ ) {
		if (!_frames[t]) {
			_frames[t] = new Image(_x_size, _y_size);
		}

		_frames[t]->fill(value);
	}
}


Image* Sequence::GetFrame(int t)
{
	if (t < 0 || t >= _t_size)
		return 0;

	return _frames[t];
}

bool Sequence::SetFrame(int t, Image *frame)
{
	if (t < 0 || t >= _t_size)
		return false;

	if (frame && (
			_x_size != frame->GetXSize() ||
			_y_size != frame->GetYSize() ))
		return false;

	_frames[t] = frame;
	return true;
}

bool Sequence::AddFrame(Image *frame)
{
	if (frame && (
			_x_size != frame->GetXSize() ||
			_y_size != frame->GetYSize() ))
		return false;

	_frames.push_back(frame);
	_t_size++;
	return true;
}

int Sequence::DropEmptyFrames()
{
	// TODO: check
	vector<Image* >::iterator i = _frames.begin();
	while (i != _frames.end()) {
		if (!i.base()) {
			i = _frames.erase(i);
		} else {
			++i;
		}
	}

	_t_size = _frames.size();

	return _t_size;
}

Sequence* Sequence::GetPatchBetweenPoints(int a_x, int a_y, int a_t, int b_x, int b_y, int b_t)
{
	if (a_x > b_x || a_y > b_y || a_t > b_t) {
		return 0;
	}

	// check if whole patch is inside the image
	if (a_x < 0 || b_x >= _x_size ||
		a_y < 0 || b_y >= _y_size ||
		a_t < 0 || b_t >= _t_size) {
		return 0;
	}

	return GetPatchInternal(a_x, a_y, a_t, b_x, b_y, b_t);
}

Sequence* Sequence::GetPatchArountPoint(int center_x, int center_y, int center_t, int size)
{
	return GetPatchArountPoint(center_x, center_y, center_t, size, size, size);
}

Sequence* Sequence::GetPatchArountPoint(int center_x, int center_y, int center_t, int frame_size, int t_size)
{
	return GetPatchArountPoint(center_x, center_y, center_t, frame_size, frame_size, t_size);
}

Sequence* Sequence::GetPatchArountPoint(int center_x, int center_y, int center_t, int x_size, int y_size, int t_size)
{
	if (x_size <= 0 || y_size <= 0 || t_size <= 0) {
		return 0;
	}

	// force 'x_size', 'y_size' and 't_size' to be odd
	if (!IsOddNumber(x_size)) x_size--;
	if (!IsOddNumber(y_size)) y_size--;
	if (!IsOddNumber(t_size)) t_size--;

	// calculate left-top and bottom-right points of the patch
	int x_offset = x_size / 2;
	int y_offset = y_size / 2;
	int t_offset = t_size / 2;
	int a_x = center_x - x_offset;
	int a_y = center_y - y_offset;
	int b_x = center_x + x_offset;
	int b_y = center_y + y_offset;
	int a_t = center_t - t_offset;
	int b_t = center_t + t_offset;

	// check if whole patch is inside the image
	if (a_x < 0 || b_x >= _x_size ||
		a_y < 0 || b_y >= _y_size ||
		a_t < 0 || b_t >= _t_size) {
		return 0;
	}

	return GetPatchInternal(a_x, a_y, a_t, b_x, b_y, b_t);
}


// TODO: remove as depricated
Sequence* Sequence::compensate_motion_pixelwise(std::vector<OpticalFlowContainer*> optical_flow, int start_frame, int end_frame, bool crop)
{
	// TODO: check optical_flow size
	// TODO: get constant frame as a parameter

	start_frame = std::max(std::min(start_frame, _t_size - 1), 0);
	end_frame = std::max(std::min(end_frame, _t_size - 1), 0);

	if (start_frame == end_frame) {
		// TODO: return single constant frame
	}

	Sequence* compensated = new Sequence(_x_size, _y_size, _t_size); //end_frame - start_frame + 1

	for (int f = start_frame; f < end_frame; f++) {
		// f - frame to compensate
		float* flow_x = optical_flow[f]->get_flow_x();
		float* flow_y = optical_flow[f]->get_flow_y();

		for (int x = 0; x < _x_size; x++) {
			for (int y = 0; y < _y_size; y++) {
				int ind = y * _x_size + x;
				int nx = x + std::floor(flow_x[ind] * 2);
				int ny = y + std::floor(flow_y[ind] * 2);

				float value;
				if (nx < 0 || ny < 0 || nx >= _x_size || ny >= _y_size) {
					value = 0;
				} else {
					value = GetPixelValue(nx, ny, f + 1);
				}

				compensated->SetPixelValue(x, y, f, value);
			}
		}
	}

	for (int i = 0; i < _t_size; i++) {
		if (i >= start_frame && i <end_frame)
			continue;

		Image *frame = new Image(*GetFrame(i));
		compensated->SetFrame(i, frame);
	}

	return compensated;
}

/* private */

Sequence* Sequence::GetPatchInternal(int a_x, int a_y, int a_t, int b_x, int b_y, int b_t)
{
	int x_size = b_x - a_x + 1;
	int y_size = b_y - a_y + 1;
	int t_size = b_t - a_t + 1;

	Sequence* patch = new Sequence(x_size, y_size, t_size);

	for (int t = 0; t < t_size; t++) {
		Image* frame = _frames[a_t + t];
		if (frame != 0) {
			Image* frame_patch = frame->GetPatchBetweenPoints(a_x, a_y, b_x, b_y);
			patch->SetFrame(t, frame_patch);
		}
	}

	return patch;
}

inline bool Sequence::IsOddNumber(int number)
{
	return number % 2 == 1;
}




