/*
 * visualization.h
 *
 *  Created on: Aug 27, 2013
 *      Author: Vadim Fedorov
 */

#ifndef VISUALIZATION_H_
#define VISUALIZATION_H_

#include "sequence.h"
#include "sequence_mask.h"
#include "image.h"
#include "image_mask.h"
#include "shape.h"

using namespace std;

class Visualization
{
public:

	/*
	 * Sets all masked points to white (255) and unmasked - to black (0)
	 */
	static Sequence<float>* mask_to_greyscale(const SequenceMask &mask);

	/*
	 * Sets all masked points to white (255) and unmasked - to black (0)
	 */
	static Image<float>* mask_to_greyscale(const ImageMask &mask);

	/*
	 * Translates range [0.0; 1.0] into the range [0; 255]
	 * Might be used to draw probabilities or 'soft' masks
	 */
	static Sequence<float>* probability_to_greyscale(const Sequence<float> &probabilities);

	/*
	 * Translates range [0.0; 1.0] into the range [0; 255]
	 * Might be used to draw probabilities or 'soft' masks
	 */
	static Image<float>* probability_to_greyscale(const Image<float> &probabilities);
};




#endif /* VISUALIZATION_H_ */
