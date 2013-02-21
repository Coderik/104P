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

class Sequence
{
public:
	Sequence(int x_size, int y_size, int t_size, float value);
	Sequence(int x_size, int y_size, int t_size);
	Sequence(int x_size, int y_size);
	Sequence(Image *first_frame);
	~Sequence();

	int GetXSize();
	int GetYSize();
	int GetTSize();
	double GetPixelValue(int x, int y, int t);
	void SetPixelValue(int x, int y, int t, double value);
	void fill(float value);
	Image* GetFrame(int t);
	bool SetFrame(int t, Image *frame);
	bool AddFrame(Image *frame);
	int DropEmptyFrames();
	Sequence* GetPatchBetweenPoints(int a_x, int a_y, int a_t, int b_x, int b_y, int b_t);
	Sequence* GetPatchArountPoint(int center_x, int center_y, int center_t, int size);
	Sequence* GetPatchArountPoint(int center_x, int center_y, int center_t, int frame_size, int t_size);
	Sequence* GetPatchArountPoint(int center_x, int center_y, int center_t, int x_size, int y_size, int t_size);
	Sequence* compensate_motion_pixelwise(std::vector<OpticalFlowContainer*> optical_flow, int start_frame, int end_frame, bool crop = false);

private:
	int _x_size, _y_size, _t_size;
	vector<Image* > _frames;

	Sequence* GetPatchInternal(int a_x, int a_y, int a_t, int b_x, int b_y, int b_t);
	inline bool IsOddNumber(int number);
};


#endif /* SEQUENCE_H_ */
