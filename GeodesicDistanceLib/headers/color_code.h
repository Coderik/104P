/*
 * color_code.h
 *
 *  Created on: Feb 8, 2013
 *      Author: Vadim Fedorov
 */

#ifndef COLOR_CODE_H_
#define COLOR_CODE_H_

#include <vector>
#include <math.h>

#include "i_optical_flow_code.h"

using namespace std;

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
	vector<vector<int> > _color_wheel;

	void set_color(int r, int g, int b, int index);
	void make_color_wheel();
};


#endif /* COLOR_CODE_H_ */
