/*
 * FastMarchAlgorithm.cpp
 *
 *  Created on: Nov 15, 2012
 *      Author: Vadim Fedorov
 */

#include <stdlib.h>
#include <stdio.h>
#include <vector>

#include "headers/fast_march_algorithm.h"
#include "headers/image.h"

using namespace std;

FastMarchAlgorithm::FastMarchAlgorithm(IDistanceCalculation *distanceCalculation)
{
	_distanceCalculation = distanceCalculation;
}




