/*
 * gaussian_weights.h
 *
 *  Created on: Nov 22, 2013
 *      Author: Vadim Fedorov
 */

#ifndef GAUSSIAN_WEIGHTS_H_
#define GAUSSIAN_WEIGHTS_H_

#include <vector>
#include "image.h"
#include "sequence.h"

using namespace std;

namespace GaussianWeights {

static const unsigned int SEGMENTS = 1;	// NOTE: 1 means no integration

Sequence<float> calculate_anisotropic_3d(unsigned int size_x, unsigned int size_y, unsigned int size_z, float sigma_x, float sigma_y, float sigma_z);
Sequence<float> calculate_isotropic_3d(unsigned int size, float sigma);

Image<float> calculate_anisotropic_2d(unsigned int size_x, unsigned int size_y, float sigma_x, float sigma_y);
Image<float> calculate_isotropic_2d(unsigned int size, float sigma);

Image<float> calculate_1d(unsigned int size, float sigma);

}	// namespace GaussianWeights



#endif /* GAUSSIAN_WEIGHTS_H_ */
