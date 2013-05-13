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
	Sequence(int x_size, int y_size, int t_size, T value);
	Sequence(int x_size, int y_size, int t_size);
	Sequence(int x_size, int y_size);
	Sequence(Image<T> *first_frame);
	Sequence(Sequence& source);
	~Sequence();

	int get_size_x();
	int get_size_y();
	int get_size_t();
	Shape get_size();
	void set_coordinates(Point coordinates);
	Point get_coordinates();
	T get_value(int x, int y, int t);
	bool try_get_value(int x, int y, int t, T& value);
	void set_value(int x, int y, int t, T value);
	void fill(T value);
	Image<T>* get_frame(int t);
	bool set_frame(int t, Image<T> *frame);
	bool add_frame(Image<T> *frame);
	int drop_empty_frames();
	Sequence<T>* get_patch_between_points(int a_x, int a_y, int a_t, int b_x, int b_y, int b_t);
	Sequence<T>* get_patch_around_point(int center_x, int center_y, int center_t, int size);
	Sequence<T>* get_patch_around_point(int center_x, int center_y, int center_t, int frame_size, int t_size);
	Sequence<T>* get_patch_around_point(int center_x, int center_y, int center_t, int x_size, int y_size, int t_size);

protected:
	int _x_size, _y_size, _t_size;
	Point _coordinates;
	vector<Image<T>* > _frames;

	Sequence<T>* get_patch_internal(int a_x, int a_y, int a_t, int b_x, int b_y, int b_t);
	inline bool is_odd_number(int number);
};


#endif /* SEQUENCE_H_ */
