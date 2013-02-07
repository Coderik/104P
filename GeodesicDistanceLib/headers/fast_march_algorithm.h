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

	virtual vector<vector<float> > CalculateDistanceMatrix(Image *image, int initial_x, int initial_y);
	virtual vector<vector<vector<float> > > CalculateDistanceMatrix(Sequence *sequence, int initial_x, int initial_y, int initial_t);
private:
	IDistanceCalculation *_distanceCalculation;
};


