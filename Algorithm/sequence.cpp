/*
 * Sequence.cpp
 *
 *  Created on: Nov 26, 2012
 *      Author: Vadim Fedorov
 */

#include "headers/sequence.h"

template <class T>
Sequence<T>::Sequence(int x_size, int y_size, int t_size, T value)
{
	_x_size = x_size;
	_y_size = y_size;
	_t_size = t_size;

	_frames = vector<Image<T>* >(_t_size);

	fill(value);
}


template <class T>
Sequence<T>::Sequence(int x_size,int y_size,int t_size)
{
	_x_size = x_size;
	_y_size = y_size;
	_t_size = t_size;

	_frames = vector<Image<T>* >(_t_size);
}


// TODO: test it
template <class T>
Sequence<T>::Sequence(int x_size,int y_size)
{
	_x_size = x_size;
	_y_size = y_size;
	_t_size = 0;

	_frames = vector<Image<T>* >();
}


template <class T>
Sequence<T>::Sequence(Image<T> *first_frame)
{
	_x_size = first_frame->get_size_x();
	_y_size = first_frame->get_size_y();
	_t_size = 1;

	_frames = vector<Image<T>* >(1, first_frame);
}

template <class T>
Sequence<T>::Sequence(Sequence<T>& source)
{
	_x_size = source.get_size_x();
	_y_size = source.get_size_y();
	_t_size = source.get_size_t();

	_frames = vector<Image<T>* >(_t_size);
	for (int i = 0; i < _t_size; i++) {
		Image<T> *frame = source.get_frame(i);
		if (frame) {
			_frames[i] = new Image<T>(*frame);
		}
	}
}


template <class T>
Sequence<T>::~Sequence()
{
	// TODO: check destructors of frames
	_frames.clear();
}


template <class T>
int Sequence<T>::get_size_x() const
{
	return _x_size;
}


template <class T>
int Sequence<T>::get_size_y() const
{
	return _y_size;
}


template <class T>
int Sequence<T>::get_size_t() const
{
	return _t_size;
}


template <class T>
Shape Sequence<T>::get_size() const
{
	return Shape(_x_size, _y_size, _t_size);
}


template <class T>
void Sequence<T>::set_coordinates(Point coordinates)
{
	_coordinates = coordinates;
}


template <class T>
Point Sequence<T>::get_coordinates()
{
	return _coordinates;
}


/*
 * Note: if outside the range, returns default value.
 */
template <class T>
T Sequence<T>::get_value(int x, int y, int t) const
{
	if (t < 0 || t >= _t_size || !_frames[t])
		return T();

	return _frames[t]->get_value(x, y);
}


/*
 * Note: if outside the range, returns default value.
 */
template <class T>
T Sequence<T>::get_value(Point p) const
{
	if (p.t < 0 || p.t >= _t_size || !_frames[p.t])
		return T();

	return _frames[p.t]->get_value(p.x, p.y);
}


template <class T>
bool Sequence<T>::try_get_value(int x, int y, int t, T& value)
{
	if (t < 0 || t >= _t_size || !_frames[t])
		return false;

	return _frames[t]->try_get_value(x, y, value);
}


template <class T>
void Sequence<T>::set_value(int x, int y, int t, T value)
{
	if (t < 0 || t >= _t_size)
		return;

	if (!_frames[t]) {
		_frames[t] = new Image<T>(_x_size, _y_size);
	}

	_frames[t]->set_value(x, y, value);
}


template <class T>
void Sequence<T>::fill(T value)
{
	for (int t = 0; t < _t_size; t++ ) {
		if (!_frames[t]) {
			_frames[t] = new Image<T>(_x_size, _y_size);
		}

		_frames[t]->fill(value);
	}
}


template <class T>
Image<T>* Sequence<T>::get_frame(int t)
{
	if (t < 0 || t >= _t_size)
		return 0;

	return _frames[t];
}


template <class T>
Image<T> Sequence<T>::get_frame(int t) const
{
	if (t < 0 || t >= _t_size)
		return Image<T>(_x_size, _y_size, T());

	return Image<T>(*_frames[t]);
}


template <class T>
bool Sequence<T>::set_frame(int t, Image<T> *frame)
{
	if (t < 0 || t >= _t_size)
		return false;

	if (frame && (
			_x_size != frame->get_size_x() ||
			_y_size != frame->get_size_y() ))
		return false;

	_frames[t] = frame;
	return true;
}


template <class T>
bool Sequence<T>::add_frame(Image<T> *frame)
{
	if (frame && (
			_x_size != frame->get_size_x() ||
			_y_size != frame->get_size_y() ))
		return false;

	_frames.push_back(frame);
	_t_size++;
	return true;
}


template <class T>
int Sequence<T>::drop_empty_frames()
{
	// TODO: check
	typename vector<Image<T>* >::iterator i = _frames.begin();
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


template <class T>
Sequence<T>* Sequence<T>::get_patch_between_points(int a_x, int a_y, int a_t, int b_x, int b_y, int b_t)
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

	return get_patch_internal(a_x, a_y, a_t, b_x, b_y, b_t);
}


template <class T>
Sequence<T>* Sequence<T>::get_patch_around_point(int center_x, int center_y, int center_t, int size)
{
	return get_patch_around_point(center_x, center_y, center_t, size, size, size);
}


template <class T>
Sequence<T>* Sequence<T>::get_patch_around_point(int center_x, int center_y, int center_t, int frame_size, int t_size)
{
	return get_patch_around_point(center_x, center_y, center_t, frame_size, frame_size, t_size);
}


template <class T>
Sequence<T>* Sequence<T>::get_patch_around_point(int center_x, int center_y, int center_t, int x_size, int y_size, int t_size)
{
	if (x_size <= 0 || y_size <= 0 || t_size <= 0) {
		return 0;
	}

	// force 'x_size', 'y_size' and 't_size' to be odd
	if (!is_odd_number(x_size)) x_size--;
	if (!is_odd_number(y_size)) y_size--;
	if (!is_odd_number(t_size)) t_size--;

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

	return get_patch_internal(a_x, a_y, a_t, b_x, b_y, b_t);
}


/* private */
template <class T>
Sequence<T>* Sequence<T>::get_patch_internal(int a_x, int a_y, int a_t, int b_x, int b_y, int b_t)
{
	int x_size = b_x - a_x + 1;
	int y_size = b_y - a_y + 1;
	int t_size = b_t - a_t + 1;

	Sequence<T>* patch = new Sequence<T>(x_size, y_size, t_size);

	for (int t = 0; t < t_size; t++) {
		Image<T>* frame = _frames[a_t + t];
		if (frame != 0) {
			Image<T>* frame_patch = frame->get_patch_between_points(a_x, a_y, b_x, b_y);
			patch->set_frame(t, frame_patch);
		}
	}

	patch->set_coordinates(Point(a_x, a_y, a_t));

	return patch;
}


template <class T>
inline bool Sequence<T>::is_odd_number(int number)
{
	return number % 2 == 1;
}


// Explicit instantiations
template class Sequence<float>;
template class Sequence<bool>;
template class Sequence<Point>;




