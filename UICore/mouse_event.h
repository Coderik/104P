/*
 * mouse_event.h
 *
 *  Created on: Jun 10, 2013
 *      Author: Vadim Fedorov
 */

#ifndef MOUSE_EVENT_H_
#define MOUSE_EVENT_H_

#include <gdkmm/types.h>

struct MouseEvent
{
public:
	MouseEvent(int x, int y, unsigned int modifiers) {
		this->x = x;
		this->y = y;
		this->t = 0;
		this->modifiers = modifiers;
	}


	MouseEvent(int x, int y, int t, unsigned int modifiers) {
		this->x = x;
		this->y = y;
		this->t = t;
		this->modifiers = modifiers;
	}


	int x, y, t;
	unsigned int modifiers;

	static const unsigned int SHIFT_MODIFIER = GDK_SHIFT_MASK;
	static const unsigned int CONTROL_MODIFIER = GDK_CONTROL_MASK;
	static const unsigned int ALT_MODIFIER = GDK_MOD1_MASK;		//Note: it depends on the modifier mapping, but normally it is the Alt key
};


#endif /* MOUSE_EVENT_H_ */
