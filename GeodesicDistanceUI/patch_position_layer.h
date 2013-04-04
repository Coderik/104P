/*
 * patch_position_layer.h
 *
 *  Created on: Apr 3, 2013
 *      Author: Vadim Fedorov
 */

#ifndef PATCH_POSITION_LAYER_H_
#define PATCH_POSITION_LAYER_H_

#include <vector>

#include "layer.h"
#include "point.h"

using namespace std;

class Patch_Position_Layer : public Layer
{
public:
	Patch_Position_Layer(string id, string display_name);
	virtual ~Patch_Position_Layer();

	void set_current_time(int time);
	void set_patch_size(int patch_size);
	void set_slice_origin(Point slice_origin);
	void clear_slice_origins();

	virtual void draw(const Cairo::RefPtr<Cairo::Context>& context);

private:
	int _current_time;
	int _patch_size;
	vector<Point* > _origins;

};



#endif /* PATCH_POSITION_LAYER_H_ */
