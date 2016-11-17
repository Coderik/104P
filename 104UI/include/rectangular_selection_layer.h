/**
 * Copyright (C) 2016, Vadim Fedorov <coderiks@gmail.com>
 *
 * This program is free software: you can use, modify and/or
 * redistribute it under the terms of the simplified BSD
 * License. You should have received a copy of this license along
 * this program. If not, see
 * <http://www.opensource.org/licenses/bsd-license.html>.
 */

/// Created on: Apr 30, 2013

#ifndef RECTANGULAR_SELECTION_LAYER_H_
#define RECTANGULAR_SELECTION_LAYER_H_

#include "layer.h"

class RectangularSelectionLayer : public Layer
{
public:
	RectangularSelectionLayer(std::string id, std::string display_name);
	virtual ~RectangularSelectionLayer();

	void set_data(int x, int y, int width, int height, int timestamp);
	void set_current_time(int time);

	virtual void draw(const Cairo::RefPtr<Cairo::Context>& context);

private:
	int _x, _y, _width, _height, _timestamp;
	int _current_time;
};



#endif /* RECTANGULAR_SELECTION_LAYER_H_ */
