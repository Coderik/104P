/*
 * rectangular_selection_layer.h
 *
 *  Created on: Apr 30, 2013
 *      Author: Vadim Fedorov
 */

#ifndef RECTANGULAR_SELECTION_LAYER_H_
#define RECTANGULAR_SELECTION_LAYER_H_

#include "layer.h"

using namespace std;

class RectangularSelectionLayer : public Layer
{
public:
	RectangularSelectionLayer(string id, string display_name);
	virtual ~RectangularSelectionLayer();

	void set_data(int x, int y, int width, int height, int timestamp);
	void set_current_time(int time);

	virtual void draw(const Cairo::RefPtr<Cairo::Context>& context);

private:
	int _x, _y, _width, _height, _timestamp;
	int _current_time;
};



#endif /* RECTANGULAR_SELECTION_LAYER_H_ */
