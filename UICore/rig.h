/*
 * rig.h
 *
 *  Created on: Apr 10, 2013
 *      Author: Vadim Fedorv
 */

#ifndef RIG_H_
#define RIG_H_

#include <gdk/gdk.h>
#include "i_hull.h"
#include "mouse_event.h"

/*
 * The base class for every specific Rig, which defines interface that the Hull uses
 * to interact with a Rig. Methods of that interface should be treated like slots.
 * There ia a default dummy implementation, therefore one can override in derived class
 * only those methods, which are useful for a specific Rig and just ignore the others.
 */
class Rig
{
public:
	virtual ~Rig() {};

	virtual void initialize(IHull *hull) = 0;

	virtual void activate() {};
	virtual void deactivate() {};

	virtual void sequence_changed() {};
	virtual void optical_flow_changed() {};
	virtual void current_time_changed() {};

	/* TODO: in the case of multiple possible options of user interaction these methods
	 * must be generalized e.g. by moving to separate User_Interaction class
	 */
	virtual void left_button_pressed(MouseEvent event) {};
	virtual void left_button_drag(MouseEvent event) {};
	virtual void left_button_released(MouseEvent event) {};
	virtual void key_pressed(GdkEventKey* event) {};

protected:
	IHull *_hull;
};


#endif /* RIG_H_ */
