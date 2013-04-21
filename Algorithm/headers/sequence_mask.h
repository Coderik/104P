/*
 * sequence_mask.h
 *
 *  Created on: Apr 20, 2013
 *      Author: Vadim Fedorov
 */

#ifndef SEQUENCE_MASK_H_
#define SEQUENCE_MASK_H_

#include "sequence.h"

class Sequence_Mask : public Sequence<bool>
{
public:
	Sequence_Mask(int x_size, int y_size, int t_size, bool value);
	Sequence_Mask(int x_size, int y_size, int t_size);
	Sequence_Mask(int x_size, int y_size);
	Sequence_Mask(Image<bool> *first_frame);
	~Sequence_Mask();

};


#endif /* SEQUENCE_MASK_H_ */