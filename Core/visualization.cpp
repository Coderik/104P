/*
 * visualization.cpp
 *
 *  Created on: Aug 27, 2013
 *      Author: Vadim Fedorov
 */

#include "visualization.h"

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


Sequence<float>* Visualization::probability_to_greyscale(const Sequence<float> &probabilities)
{
	Shape size = probabilities.get_size();
	Sequence<float> *result = new Sequence<float>(size.size_x, size.size_y, size.size_t, 0.0);

	for (unsigned int t = 0; t < size.size_t; t++) {
		for (unsigned int y = 0; y < size.size_y; y++) {
			for (unsigned int x = 0; x < size.size_x; x++) {
				float value = fmax(0.0, fmin(1.0, probabilities.get_value(x, y, t)));
				result->set_value(x, y, t, 255.0 * value);
			}
		}
	}

	return result;
}


Image<float>* Visualization::probability_to_greyscale(const Image<float> &probabilities)
{
	Shape size = probabilities.get_size();
	Image<float> *result = new Image<float>(size.size_x, size.size_y, 0.0);

	for (unsigned int y = 0; y < size.size_y; y++) {
		for (unsigned int x = 0; x < size.size_x; x++) {
			float value = fmax(0.0, fmin(1.0, probabilities.get_value(x, y)));
			result->set_value(x, y, 255.0 * value);
		}
	}

	return result;
}



