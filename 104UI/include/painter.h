/*
 * painter.h
 *
 *  Created on: Mar 13, 2014
 *      Author: Vadim Fedorov
 */

#ifndef PAINTER_H_
#define PAINTER_H_

#include "sequence.h"
#include "layer.h"

using namespace std;

class Painter
{
public:
	void set_canvas(const Sequence<float> &sequence);
	void add_layer(Layer *layer, string id);
	Layer* remove_layer(string id);
	void draw(string file_name);
};




#endif /* PAINTER_H_ */
