/*
 * visualization.cpp
 *
 *  Created on: Aug 27, 2013
 *      Author: Vadim Fedorov
 */

#include "headers/visualization.h"

Sequence<float>* Visualization::mask_to_greyscale(const SequenceMask &mask)
{
	Shape size = mask.get_size();
	Sequence<float> *result = new Sequence<float>(size.size_x, size.size_y, size.size_t, 0.0);

	SequenceMask::iterator it;
	for (it = mask.begin(); it != mask.end(); ++it) {
		result->set_value(it->x, it->y, it->t, 255.0);
	}

	return result;
}


Image<float>* Visualization::mask_to_greyscale(const ImageMask &mask)
{
	Shape size = mask.get_size();
	Image<float> *result = new Image<float>(size.size_x, size.size_y, 0.0);

	ImageMask::iterator it;
	for (it = mask.begin(); it != mask.end(); ++it) {
		result->set_value(it->x, it->y, 255.0);
	}

	return result;
}



