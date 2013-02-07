/*
 * optical_flow_container.cpp
 *
 *  Created on: Feb 7, 2013
 *      Author: Vadim Fedorov
 */

#include "headers/optical_flow_container.h"

OpticalFlowContainer::OpticalFlowContainer()
{
	_flow_x = 0;
	_flow_y = 0;
	_size_x = -1;
	_size_y = -1;
}


OpticalFlowContainer::~OpticalFlowContainer()
{
	//delete[] _flow_x;
	//delete[] _flow_y;
}

float* OpticalFlowContainer::get_flow_x()
{
	return _flow_x;
}


float* OpticalFlowContainer::get_flow_y()
{
	return _flow_y;
}


int OpticalFlowContainer::get_size_x()
{
	return _size_x;
}


int OpticalFlowContainer::get_size_y()
{
	return _size_y;
}


void OpticalFlowContainer::set_flow(float *flow_x, float *flow_y, int size_x, int size_y)
{
	_flow_x = flow_x;
	_flow_y = flow_y;
	_size_x = size_x;
	_size_y = size_y;
}


bool OpticalFlowContainer::contains_data()
{
	return (_flow_x && _flow_y);
}


