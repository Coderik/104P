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

#ifndef COLOR_CODE_H_
#define COLOR_CODE_H_

#include <vector>
#include <math.h>

#include "i_optical_flow_code.h"

/**
 * Color encoding of flow vectors
 * adapted from the color circle idea described at
 * http://members.shaw.ca/quadibloc/other/colint.htm
 */
class ColorCode : public IOptivalFlowCode
{
public:
	ColorCode();
	virtual ~ColorCode();

	virtual void get_color(float fx, float fy, char *color);

private:
	int _colors_count;
	std::vector<std::vector<int> > _color_wheel;

	void set_color(int r, int g, int b, int index);
	void make_color_wheel();
};


#endif /* COLOR_CODE_H_ */
