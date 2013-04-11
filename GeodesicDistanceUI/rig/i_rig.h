/*
 * i_rig.h
 *
 *  Created on: Apr 10, 2013
 *      Author: Vadim Fedorv
 */

#ifndef I_RIG_H_
#define I_RIG_H_

class IRig
{
public:
	virtual ~IRig() {};

	virtual void activate() = 0;
	virtual void deactivate() = 0;

	virtual void sequence_changed() = 0;
	virtual void optical_flow_changed() = 0;
	virtual void current_time_changed() = 0;

	// TODO: replace with more generich approach (e.g. User_Interaction class)
	virtual void left_button_pressed(int x, int y) = 0;
};


#endif /* I_RIG_H_ */
