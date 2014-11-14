/*
 * image.h
 *
 *  Created on: Nov 5, 2012
 *      Author: Vadim Fedorov
 */

#ifndef IMAGE_H_
#define IMAGE_H_

#include <algorithm>
#include "shape.h"
#include "point.h"

template <class T = float>
class Image
{
public:
	Image();
	Image(int size_x,int size_y);
	Image(int size_x,int size_y, T value);
	Image(const Image &source);
	virtual ~Image();

	Image<T>& operator= (const Image<T> &other);

	int get_size_x() const;
	int get_size_y() const;
	Shape get_size() const;
	void set_coordinates(Point coordinates);
	Point get_coordinates();
	virtual T get_value(int x, int y) const;
	virtual T get_value(Point p) const;
	bool try_get_value(int x, int y, T& value) const;
	virtual void set_value(int x, int y, T value);
	virtual void set_value(Point p, T value);
	void fill(T value);
	Image<T>* get_patch_between_points(int a_x, int a_y, int b_x, int b_y);
	Image<T>* get_patch_around_point(int center_x, int center_y, int size_x, int size_y);
	Image<T>* get_patch_around_point(int center_x, int center_y, int size);
	int get_raw_data_length() const;
	const T* get_raw_data() const;
	T* get_raw_data();
protected:
	int _size_x, _size_y;
	T *_points;
	Point _coordinates;
	inline int get_index(int x, int y) const;
	inline bool is_odd_number(int number);
	Image<T>* get_patch_internal(int a_x, int a_y, int b_x, int b_y);
};

// NOTE: include implementation, because Image is a template
#include "../image.hpp"

#endif /* IMAGE_H_ */
