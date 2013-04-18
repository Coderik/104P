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

vector<vector<float> > FastMarchAlgorithm::CalculateDistanceMatrix(Image *image, int initial_x, int initial_y)
{
	// TODO: implement

	int x_size = image->GetXSize();
	int y_size = image->GetYSize();

	// allocate memory and set default values
	vector<vector<float> > distances (y_size, vector<float> (x_size, 999.0));

	return distances;
}

vector<vector<vector<float> > > FastMarchAlgorithm::CalculateDistanceMatrix(Sequence *sequence, int initial_x, int initial_y, int initial_t)
{
	// TODO: implement

	int x_size = sequence->GetXSize();
	int y_size = sequence->GetYSize();
	int t_size = sequence->GetTSize();

	// allocate memory and set default values
	vector<vector<vector<float> > > distances (t_size, vector<vector<float> > (y_size, vector<float> (x_size, 999.0)));

	return distances;
}




