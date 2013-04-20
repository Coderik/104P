/*
 * FastMarchAlgorithm.h
 *
 *  Created on: Nov 15, 2012
 *      Author: Vadim Fedorov
 */

#include "image.h"
#include "i_distance_calculation.h"
#include "abstract_marching_algorithm.h"

class FastMarchAlgorithm : public AbstractMarchingAlgorithm
{
public:
	FastMarchAlgorithm(IDistanceCalculation *distanceCalculation);
	virtual ~FastMarchAlgorithm() {}
	
private:
	IDistanceCalculation *_distanceCalculation;
};


