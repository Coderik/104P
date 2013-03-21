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

class Image
{
public:
	Image();
	Image(int x_size,int y_size);
	Image(int x_size,int y_size, float value);
	Image(Image &source);
	~Image();

	int GetXSize() const;
	int GetYSize() const;
	Shape get_size() const;
	float GetPixelValue(int x, int y) const;
	void SetPixelValue(int x, int y, double value);
	void fill(float value);
	Image* GetPatchBetweenPoints(int a_x, int a_y, int b_x, int b_y);
	Image* GetPatchArountPoint(int center_x, int center_y, int x_size, int y_size);
	Image* GetPatchArountPoint(int center_x, int center_y, int size);
	char* GetRawDataRgb();
	int GetRawDataLength();
	float* GetRawData();
private:
	int _x_size, _y_size;
	float *_points;
	inline int GetIndex(int x, int y) const;
	inline bool IsOddNumber(int number);
	Image* GetPatchInternal(int a_x, int a_y, int b_x, int b_y);
};


#endif /* IMAGE_H_ */
