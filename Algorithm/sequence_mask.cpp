/*
 * sequence_mask.cpp
 *
 *  Created on: Apr 20, 2013
 *      Author: Vadim Fedorov
 */

#include "headers/sequence_mask.h"

Sequence_Mask::Sequence_Mask(int x_size, int y_size, int t_size, bool value)
	: Sequence<bool>(x_size, y_size, t_size, value)
{

}


Sequence_Mask::Sequence_Mask(int x_size, int y_size, int t_size)
	: Sequence<bool>(x_size, y_size, t_size)
{

}


Sequence_Mask::Sequence_Mask(int x_size, int y_size)
	: Sequence<bool>(x_size, y_size)
{

}


Sequence_Mask::Sequence_Mask(Image<bool> *first_frame)
	: Sequence<bool>(first_frame)
{

}


Sequence_Mask::~Sequence_Mask()
{

}