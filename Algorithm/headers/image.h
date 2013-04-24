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
	Image(int x_size,int y_size);
	Image(int x_size,int y_size, T value);
	Image(Image &source);
	~Image();

	int GetXSize() const;
	int GetYSize() const;
	Shape get_size() const;
	void set_coordinates(Point coordinates);
	Point get_coordinates();
	T get_value(int x, int y) const;
	bool try_get_value(int x, int y, T& value) const;
	void set_value(int x, int y, T value);
	void fill(T value);
	Image<T>* GetPatchBetweenPoints(int a_x, int a_y, int b_x, int b_y);
	Image<T>* GetPatchArountPoint(int center_x, int center_y, int x_size, int y_size);
	Image<T>* GetPatchArountPoint(int center_x, int center_y, int size);
	int GetRawDataLength();
	T* GetRawData();
protected:
	int _x_size, _y_size;
	T *_points;
	Point _coordinates;
	inline int GetIndex(int x, int y) const;
	inline bool IsOddNumber(int number);
	Image<T>* GetPatchInternal(int a_x, int a_y, int b_x, int b_y);
};

// NOTE: include implementation, because Image is a template
//#include "../image.cpp"

#endif /* IMAGE_H_ */
