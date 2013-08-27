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

class Visualization
{
public:

	/*
	 * Sets all masked points to white (255.0) and unmasked - to black (0.0)
	 */
	static Sequence<float>* mask_to_greyscale(const SequenceMask &mask);

	/*
	 * Sets all masked points to white (255.0) and unmasked - to black (0.0)
	 */
	static Image<float>* mask_to_greyscale(const ImageMask &mask);
};




#endif /* VISUALIZATION_H_ */
