/**
 * Copyright (C) 2016, Vadim Fedorov <coderiks@gmail.com>
 *
 * This program is free software: you can use, modify and/or
 * redistribute it under the terms of the simplified BSD
 * License. You should have received a copy of this license along
 * this program. If not, see
 * <http://www.opensource.org/licenses/bsd-license.html>.
 */

/// Created on: Aug 27, 2013

#ifndef VISUALIZATION_H_
#define VISUALIZATION_H_

#include "sequence.h"
#include "mask_sequence.h"
#include "image.h"
#include "mask.h"
#include "shape.h"

class Visualization
{
public:

	/*
	 * Sets all masked points to white (255) and unmasked - to black (0)
	 */
	static Sequence<float> mask_to_greyscale(const MaskSequenceFx &mask);

	/*
	 * Sets all masked points to white (255) and unmasked - to black (0)
	 */
	static Image<float> mask_to_greyscale(const MaskFx &mask);

	/*
	 * Translates range [0.0; 1.0] into the range [0; 255]
	 * Might be used to draw probabilities or 'soft' masks
	 */
	static Sequence<float> probability_to_greyscale(const SequenceFx<float> &probabilities);

	/*
	 * Translates range [0.0; 1.0] into the range [0; 255]
	 * Might be used to draw probabilities or 'soft' masks
	 */
	static Image<float> probability_to_greyscale(const ImageFx<float> &probabilities);
};




#endif /* VISUALIZATION_H_ */
