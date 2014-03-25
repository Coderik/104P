/*
 * rectangular_selection.h
 *
 *  Created on: Sep 29, 2013
 *      Author: Vadim Fedorov
 */

#ifndef RECTANGULAR_SELECTION_H_
#define RECTANGULAR_SELECTION_H_

#include <gtkmm/icontheme.h>
#include "selection_interaction.h"
#include "i_hull.h"
#include "sequence_mask.h"
#include "point.h"
#include "mouse_event.h"
#include "rectangular_selection_layer.h"
#include "../icons/icon_rectangular_selection.h"
#include "mask_group.h"

class RectangularSelection : public SelectionInteraction
{
public:
	RectangularSelection(IHull *hull, MaskGroup *group = 0);	// TODO: add default value 0
	virtual void left_button_pressed(MouseEvent event);
	virtual void left_button_released(MouseEvent event);
	virtual void left_button_drag(MouseEvent event);

private:
	IHull *_hull;
	Point _begin, _end;
};


#endif /* RECTANGULAR_SELECTION_H_ */
