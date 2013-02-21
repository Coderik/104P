/*
 * Image.cpp
 *
 *  Created on: Nov 5, 2012
 *      Author: Vadim Fedorov
 */

#include <stdlib.h>
#include <stdio.h>

#include "headers/image.h"

Image::Image()
{
	_x_size = 0;
	_y_size = 0;
	_points = 0;
}


Image::Image(int x_size, int y_size)
{
	_x_size = x_size;
	_y_size = y_size;

	_points = (float *)malloc(sizeof(float) * x_size * y_size);
}


Image::Image(Image &source)
{
	_x_size = source._x_size;
	_y_size = source._y_size;
	_points = (float *)malloc(sizeof(float) * _x_size * _y_size);

	// TODO: rewrite copying
	for (int i = 0; i < _x_size * _y_size; i++) {
		_points[i] = source._points[i];
	}
}


Image::~Image()
{
	if (_points) {
		free(_points);
	}
}

/* public */

float Image::GetPixelValue(int x, int y)
{
	if ( x < 0 || y < 0 || x >= _x_size || y >= _y_size || !_points) {
		return -1;
	}

	return _points[GetIndex(x,y)];
}

void Image::SetPixelValue(int x, int y, double value)
{
	if ( x < 0 || y < 0 || x >= _x_size || y >= _y_size || !_points || value < 0) {
		return;
	}

	_points[GetIndex(x,y)] = value;
}


void Image::fill(float value)
{
	std::fill_n(_points, _x_size*_y_size, value);
}


int Image::GetXSize()
{
	return _x_size;
}


int Image::GetYSize()
{
	return _y_size;
}


Image* Image::GetPatchBetweenPoints(int a_x, int a_y, int b_x, int b_y)
{
	// /* ensure that points lie inside the image */
	// a_x = std::max(std::min(a_x, _x_size - 1), 0);
	// a_y = std::max(std::min(a_y, _x_size - 1), 0);
	// b_x = std::max(std::min(b_x, _x_size - 1), 0);
	// b_y = std::max(std::min(b_y, _x_size - 1), 0);

	if (a_x >= b_x || a_y >= b_y) {
		return 0;
	}

	// check if whole patch is inside the image
	if (a_x < 0 || b_x >= _x_size ||
		a_y < 0 || b_y >= _y_size) {
		return 0;
	}

	return GetPatchInternal(a_x, a_y, b_x, b_y);
}


Image* Image::GetPatchArountPoint(int center_x, int center_y, int x_size, int y_size)
{
	if (x_size <= 0 || y_size <= 0) {
		return 0;
	}

	// force 'x_size' and 'y_size' to be odd
	if (!IsOddNumber(x_size)) x_size--;
	if (!IsOddNumber(y_size)) y_size--;

	// calculate left-top and bottom-right points of the patch
	int x_offset = x_size / 2;
	int y_offset = y_size / 2;
	int a_x = center_x - x_offset;
	int a_y = center_y - y_offset;
	int b_x = center_x + x_offset;
	int b_y = center_y + y_offset;

	// check if whole patch is inside the image
	if (a_x < 0 || b_x >= _x_size ||
		a_y < 0 || b_y >= _y_size) {
		return 0;
	}

	return GetPatchInternal(a_x, a_y, b_x, b_y);
}


Image* Image::GetPatchArountPoint(int center_x, int center_y, int size)
{
	return GetPatchArountPoint(center_x, center_y, size, size);
}

/**
 * Obsolete
 */
char* Image::GetRawDataRgb()
{
	char* raw_data = (char*)malloc(sizeof(char) * _x_size * _y_size * 3);
	for (long i = 0;i < _x_size * _y_size;i++) {
		unsigned char value = (unsigned char)_points[i];
		raw_data[3 * i] = value;
		raw_data[3 * i + 1] = value;
		raw_data[3 * i + 2] = value;
	}

	return raw_data;
}


int Image::GetRawDataLength()
{
	return _x_size * _y_size;
}


/**
 * Returns pointer to internal raw data representation.
 */
float* Image::GetRawData()
{
	return _points;
}


/* private */

inline int Image::GetIndex(int x, int y)
{
	return _x_size * y + x;
}

inline bool Image::IsOddNumber(int number)
{
	return number % 2 == 1;
}

Image* Image::GetPatchInternal(int a_x, int a_y, int b_x, int b_y)
{
	int x_size = b_x - a_x + 1;
	int y_size = b_y - a_y + 1;

	Image *patch = new Image(x_size, y_size);

	// TODO: may be add mirror effect at the border?
	for (int x = 0; x < x_size; x++) {
		for (int y = 0; y < y_size; y++) {
			float value = this->GetPixelValue(a_x + x, a_y + y);
			patch->SetPixelValue(x,y,value);
		}
	}

	return patch;
}
