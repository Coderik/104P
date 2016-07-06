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

#include "intensity_code.h"

IntensityCode::IntensityCode()
{

}


IntensityCode::~IntensityCode() {}


void IntensityCode::get_color(float fx, float fy, char *color)
{
	float len = sqrt(fx*fx + fy*fy);
	len = std::max(std::min(len, 1.0f), 0.0f);
	float intensity = 255.0f * (1 - ((len - 1) * (len - 1)));

	color[0] = color[1] = color[2] = intensity;
}

