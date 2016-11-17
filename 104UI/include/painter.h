/**
 * Copyright (C) 2016, Vadim Fedorov <coderiks@gmail.com>
 *
 * This program is free software: you can use, modify and/or
 * redistribute it under the terms of the simplified BSD
 * License. You should have received a copy of this license along
 * this program. If not, see
 * <http://www.opensource.org/licenses/bsd-license.html>.
 */

/// Created on: Mar 13, 2014

#ifndef PAINTER_H_
#define PAINTER_H_

#include "sequence.h"
#include "layer.h"

class Painter
{
public:
	void set_canvas(const Sequence<float> &sequence);
	void add_layer(Layer *layer, std::string id);
	Layer* remove_layer(std::string id);
	void draw(std::string file_name);
};




#endif /* PAINTER_H_ */
