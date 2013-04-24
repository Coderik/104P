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
#include "optical_flow_container.h"

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

	int GetXSize();
	int GetYSize();
	int GetTSize();
	Shape get_size();
	void set_coordinates(Point coordinates);
	Point get_coordinates();
	T get_value(int x, int y, int t);
	bool try_get_value(int x, int y, int t, T& value);
	void set_value(int x, int y, int t, T value);
	void fill(T value);
	Image<T>* GetFrame(int t);
	bool SetFrame(int t, Image<T> *frame);
	bool AddFrame(Image<T> *frame);
	int DropEmptyFrames();
	Sequence<T>* GetPatchBetweenPoints(int a_x, int a_y, int a_t, int b_x, int b_y, int b_t);
	Sequence<T>* GetPatchArountPoint(int center_x, int center_y, int center_t, int size);
	Sequence<T>* GetPatchArountPoint(int center_x, int center_y, int center_t, int frame_size, int t_size);
	Sequence<T>* GetPatchArountPoint(int center_x, int center_y, int center_t, int x_size, int y_size, int t_size);

protected:
	int _x_size, _y_size, _t_size;
	Point _coordinates;
	vector<Image<T>* > _frames;

	Sequence<T>* GetPatchInternal(int a_x, int a_y, int a_t, int b_x, int b_y, int b_t);
	inline bool IsOddNumber(int number);
};


#endif /* SEQUENCE_H_ */
