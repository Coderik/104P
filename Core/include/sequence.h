/*
 * Sequence.h
 *
 *  Created on: Nov 26, 2012
 *      Author: Vadim Fedorov
 */

#ifndef SEQUENCE_H_
#define SEQUENCE_H_

#include <vector>
#include "image.h"
#include "point.h"
#include "shape.h"

using namespace std;

template <class T = float>
class Sequence
{
public:
	Sequence(int size_x, int size_y, int size_t, T value);
	Sequence(int size_x, int size_y, int size_t);
	Sequence(int size_x, int size_y);
	Sequence(Shape size, T value);
	Sequence(Shape size);
	Sequence(Image<T> *first_frame);
	Sequence(const Sequence& source);
	~Sequence();

	int get_size_x() const;
	int get_size_y() const;
	int get_size_t() const;
	Shape get_size() const;
	void set_coordinates(Point coordinates);
	Point get_coordinates();
	T get_value(int x, int y, int t) const;
	T get_value(Point p) const;
	bool try_get_value(int x, int y, int t, T& value);
	void set_value(int x, int y, int t, T value);
	void fill(T value);
	Image<T>* get_frame(int t);
	Image<T> get_frame(int t) const;
	Image<T>* get_frame_as_is(int t) const;
	bool set_frame(int t, Image<T> *frame);
	bool add_frame(Image<T> *frame);
	int drop_empty_frames();
	Sequence<T>* get_patch_between_points(int a_x, int a_y, int a_t, int b_x, int b_y, int b_t);
	Sequence<T>* get_patch_around_point(int center_x, int center_y, int center_t, int size);
	Sequence<T>* get_patch_around_point(int center_x, int center_y, int center_t, int frame_size, int size_t);
	Sequence<T>* get_patch_around_point(int center_x, int center_y, int center_t, int size_x, int size_y, int size_t);

protected:
	int _size_x, _size_y, _size_t;
	Point _coordinates;
	vector<Image<T>* > _frames;

	Sequence<T>* get_patch_internal(int a_x, int a_y, int a_t, int b_x, int b_y, int b_t);
	inline bool is_odd_number(int number);
};

// NOTE: include implementation, because Sequence is a template
#include "../sequence.hpp"

#endif /* SEQUENCE_H_ */
