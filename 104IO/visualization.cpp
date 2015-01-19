/*
 * visualization.cpp
 *
 *  Created on: Aug 27, 2013
 *      Author: Vadim Fedorov
 */

#include "visualization.h"

Sequence<float> Visualization::mask_to_greyscale(const MaskSequenceFx &mask)
{
	Shape size = mask.size();
	Sequence<float> result(size, 0.0);

	Mask::iterator it;
	for (uint t = 0; t < size.size_t; t++) {
		Image<float> result_frame = result.frame(t);
		for (it = mask[t].begin(); it != mask[t].end(); ++it) {
			result_frame(*it) = 255.0;
		}
	}

	return result;
}


Image<float> Visualization::mask_to_greyscale(const MaskFx &mask)
{
	Shape size = mask.size();
	Image<float> result(size, 0.0f);

	Mask::iterator it;
	for (it = mask.begin(); it != mask.end(); ++it) {
		result(*it) = 255.0;
	}

	return result;
}


Sequence<float> Visualization::probability_to_greyscale(const SequenceFx<float> &probabilities)
{
	Shape size = probabilities.size();
	Sequence<float> result(size, 0.0f);

	for (uint t = 0; t < size.size_t; t++) {
		ImageFx<float> probabilities_frame = probabilities.frame(t);
		Image<float> result_frame = result.frame(t);
		for (uint y = 0; y < size.size_y; y++) {
			for (uint x = 0; x < size.size_x; x++) {
				float value = fmax(0.0, fmin(1.0, probabilities_frame(x, y)));
				result_frame(x, y) = 255.0 * value;
			}
		}
	}

	return result;
}


Image<float> Visualization::probability_to_greyscale(const ImageFx<float> &probabilities)
{
	Shape size = probabilities.size();
	Image<float> result(size, 0.0f);

	for (uint y = 0; y < size.size_y; y++) {
		for (uint x = 0; x < size.size_x; x++) {
			float value = fmax(0.0, fmin(1.0, probabilities(x, y)));
			result(x, y) = 255.0 * value;
		}
	}

	return result;
}



