/*
 * patch_weight_mode.h
 *
 *  Created on: Apr 9, 2013
 *      Author: Vadim Fedorov
 */

#ifndef PATCH_WEIGHT_MODE_H_
#define PATCH_WEIGHT_MODE_H_

#include <glibmm/ustring.h>

#include "sequence.h"
#include "shape.h"
#include "point.h"
#include "patch_weight_ui_container.h"

class Patch_Weight_Mode
{
public:
	enum DistanceMode
	{
		extended_space,
		patch_space,
		threshold
	};

	Patch_Weight_Mode();
	void initialize(Sequence *sequence);

private:
	Patch_Weight_UI_Container _ui;
	Sequence *_sequence, *_distances;
	vector<Glib::RefPtr<Gdk::Pixbuf> > _color_representations;
	Glib::RefPtr<Gdk::Pixbuf> _patch_slice, _empty_pixmap;
	Point _patch_center;
	int _current_time;
	int _patch_size, _patch_duration;
	int _patch_scale;
	float _distance_weight, _color_weight, _gamma;
	DistanceMode _distance_mode;

	void update_coordinates();
};



#endif /* PATCH_WEIGHT_MODE_H_ */
