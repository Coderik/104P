/*
 * intensity_code.cpp
 *
 *  Created on: Feb 8, 2013
 *      Author: Vadim Fedorov
 */

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

