/**
 * Copyright (C) 2016, Vadim Fedorov <coderiks@gmail.com>
 *
 * This program is free software: you can use, modify and/or
 * redistribute it under the terms of the simplified BSD
 * License. You should have received a copy of this license along
 * this program. If not, see
 * <http://www.opensource.org/licenses/bsd-license.html>.
 */

/// Created on: Feb 8, 2013

#include "color_code.h"

ColorCode::ColorCode()
{
	make_color_wheel();
}


ColorCode::~ColorCode()
{

}


void ColorCode::get_color(float fx, float fy, char *color)
{
	float radius = sqrt(fx * fx + fy * fy);
	float angle = atan2(-fy, -fx) / M_PI;
	float fk = (angle + 1.0) / 2.0 * (_colors_count - 1);
	int k0 = (int)fk;
	int k1 = (k0 + 1) % _colors_count;
	float f = fk - k0;
	//f = 0; // uncomment to see original color wheel
	for (int b = 0; b < 3; b++) {
		float component_0 = _color_wheel[b][k0] / 255.0;
		float component_1 = _color_wheel[b][k1] / 255.0;
		float intencity = (1 - f) * component_0 + f * component_1;

		if (radius <= 1) {
			intencity = 1 - radius * (1 - intencity); // increase saturation with radius
		} else {
			intencity *= .75; // out of range
		}

		//color[2 - b] = (int)(255.0 * intencity);
		color[b] = (int)(255.0 * intencity);
	}
}

/* private */

void ColorCode::set_color(int r, int g, int b, int index)
{
	_color_wheel[0][index] = r;
	_color_wheel[1][index] = g;
	_color_wheel[2][index] = b;
}


void ColorCode::make_color_wheel()
{
	// relative lengths of color transitions:
	// these are chosen based on perceptual similarity
	// (e.g. one can distinguish more shades between red and yellow
	//  than between yellow and green)
	int RY = 15;
	int YG = 6;
	int GC = 4;
	int CB = 11;
	int BM = 13;
	int MR = 6;

	_colors_count = RY + YG + GC + CB + BM + MR;
	_color_wheel = vector<vector<int> >(3, vector<int>(_colors_count, 0));

	int i;
	int k = 0;
	for (i = 0; i < RY; i++) set_color(255,				255*i/RY,		0,				k++);
	for (i = 0; i < YG; i++) set_color(255-255*i/YG,	255,			0,				k++);
	for (i = 0; i < GC; i++) set_color(0,				255,			255*i/GC,		k++);
	for (i = 0; i < CB; i++) set_color(0,				255-255*i/CB,	255,			k++);
	for (i = 0; i < BM; i++) set_color(255*i/BM,		0,				255,			k++);
	for (i = 0; i < MR; i++) set_color(255,				0,				255-255*i/MR,	k++);
}



