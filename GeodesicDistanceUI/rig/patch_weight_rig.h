/*
 * patch_weight_mode.h
 *
 *  Created on: Apr 9, 2013
 *      Author: Vadim Fedorov
 */

#ifndef PATCH_WEIGHT_MODE_H_
#define PATCH_WEIGHT_MODE_H_

#include <vector>

#include <glibmm/ustring.h>

#include "i_rig.h"
#include "i_hull.h"
#include "sequence.h"
#include "shape.h"
#include "point.h"
#include "patch_weight_ui_container.h"
#include "dijkstra_algorithm.h"
#include "simple_distance_calculation.h"
#include "../layer_manager.h"
#include "../patch_position_layer.h"

using namespace std;

class Patch_Weight_Rig : public IRig
{
public:
	enum DistanceMode
	{
		extended_space,
		patch_space,
		threshold
	};
	enum MotionCompensationMode
	{
		no_compensation,
		pixelwise,
		patch_shift_central,
		patch_shift_weighted
	};

	Patch_Weight_Rig(IHull *hull);
	virtual ~Patch_Weight_Rig() {}

	virtual void activate();
	virtual void deactivate();

private:
	IHull *_hull;
	Patch_Weight_UI_Container _ui;
	Sequence *_distances;
	vector<Glib::RefPtr<Gdk::Pixbuf> > _color_representations;
	Glib::RefPtr<Gdk::Pixbuf> _patch_slice, _empty_pixmap;
	Point _patch_center;
	int _patch_size, _patch_duration;
	int _patch_scale;
	float _distance_weight, _color_weight, _gamma;
	DistanceMode _distance_mode;
	MotionCompensationMode _motion_compensation_mode;

	// Slots
	void set_distance_mode();
	void set_motion_compensation_mode();
	void set_patch_zoom();
	void set_patch_size();
	void set_patch_duration();
	void set_distance_weight();
	void set_color_weight();
	void set_gamma();

	Glib::RefPtr<Gdk::Pixbuf> create_empty_pixbuf(int width, int height);
	Sequence* calculate_distances(Sequence &sequence,
								  const Point &patch_center,
								  const Shape &patch_size,
								  float distance_threshold,
								  DistanceMode mode,
								  float distance_weight,
								  float color_weight);
	Patch_Position_Layer* find_or_create_patch_position_layer(Layer_Manager *layer_manager);
	vector<Glib::RefPtr<Gdk::Pixbuf> > draw_distances_with_color(Sequence &distances, float gamma);
	Glib::RefPtr<Gdk::Pixbuf> wrap_raw_image_data(Image *image);
	Glib::RefPtr<Gdk::Pixbuf> get_distance_representaton_by_time(
									vector<Glib::RefPtr<Gdk::Pixbuf> > representation,
									int patch_time_offset,
									int current_time,
									Glib::RefPtr<Gdk::Pixbuf> empty_pixbuf);
	void show_patch_pixbuf(Glib::RefPtr<Gdk::Pixbuf> pixmap, int scale);
	void update_coordinates();
};



#endif /* PATCH_WEIGHT_MODE_H_ */
