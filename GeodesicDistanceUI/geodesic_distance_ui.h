/*
 * geodesic_distance_ui.h
 *
 *  Created on: Jan 9, 2013
 *      Author: Vadim Fedorov
 */

#ifndef GEODESIC_DISTANCE_UI_H_
#define GEODESIC_DISTANCE_UI_H_

#include <gtkmm/main.h>
#include <gtkmm/window.h>
#include <glibmm/ustring.h>
#include <gtkmm/filechooserdialog.h>
#include <giomm/file.h>
#include <glibmm/threads.h>
#include <glibmm/dispatcher.h>

#include <string.h>
#include <iostream>
#include <algorithm>

#include "ui_container.h"
#include "image.h"
#include "sequence.h"
#include "pgm_util.h"
#include "dijkstra_algorithm.h"
#include "simple_distance_calculation.h"
#include "zach_TVL1_optical_flow.h"
#include "util.h"
#include "optical_flow_io.h"
#include "optical_flow.h"
#include "optical_flow_container.h"
#include "signal_watchdog.h"
#include "point.h"
#include "shape.h"



class Geodesic_Distance_UI : public Gtk::Window
{
public:
	enum DistanceMode
	{
		extended_space,
		patch_space,
		threshold
	};

	Geodesic_Distance_UI();
	~Geodesic_Distance_UI();

protected:
	/* slots */
	void open_image();
	void open_sequence();
	void set_distance_mode();
	void set_patch_zoom();
	void set_patch_size();
	void set_patch_duration();
	void set_patch_location(int patch_center_x, int patch_center_y);
	void set_distance_weight();
	void set_color_weight();
	void set_gamma();
	void set_time();
	void restore_optical_flow();
	void show_hide_optical_flow();
	void perceive_background_worker(int responce_id);	//TODO: rename it
	bool allow_background_computation();

	bool move_selected_point(GdkEventKey* event);

private:
	static const int MAX_PATCH_SCALE = 8;

	Sequence *_sequence, *_distances;
	vector<Glib::RefPtr<Gdk::Pixbuf> > _color_representations;
	Glib::RefPtr<Gdk::Pixbuf> _patch_slice, _empty_pixmap;
	Glib::RefPtr<Gdk::Pixbuf> _optical_flow_view;
	std::vector<OpticalFlow> _optical_flow_list;
	std::string _sequence_folder;
	float _distance_weight, _color_weight, _gamma;
	int _current_time;
	int _patch_size, _patch_duration;
	Point _patch_center;
	int _patch_scale;
	int _progress_counter;
	DistanceMode _distance_mode;
	Glib::Threads::Thread *_background_worker;
	Glib::Dispatcher _work_done_dispatcher;
	Glib::Dispatcher _portion_ready_dispatcher;

	Glib::Threads::Mutex _background_work_mutex;	// covers following variables
	bool _aux_stop_background_work_flag;
	float* _aux_optical_flow_x;
	float* _aux_optical_flow_y;
	int _aux_optical_flow_index;

	UI_Container _ui;

	Sequence* calculate_distances(Sequence &sequence, const Point &patch_center, const Shape &patch_size, float distance_threshold, DistanceMode mode, float distance_weight, float color_weight);
	vector<Glib::RefPtr<Gdk::Pixbuf> > draw_distances_with_color(Sequence &distances, float gamma);

	Glib::RefPtr<Gdk::Pixbuf> WrapRawImageData(Image *image);
	Glib::RefPtr<Gdk::Pixbuf> GetDistanceRepresentatonByTime(vector<Glib::RefPtr<Gdk::Pixbuf> > representation, int patch_time_offset, int current_time, Glib::RefPtr<Gdk::Pixbuf> empty_pixbuf);
	void ShowPatchPixbuf(Glib::RefPtr<Gdk::Pixbuf> pixmap, int scale);
	void update_image_control(int current_time);
	Glib::RefPtr<Gdk::Pixbuf> CreateEmptyPixbuf(int width, int height);
	void ShowStatusMessage(std::string message);
	void UpdateCoordinates();
	void store_optical_flow(OpticalFlow &flow, int index);
	void calculate_optical_flow();
	void begin_calculate_optical_flow();
	void end_calculate_optical_flow();
	void cancel_calculate_optical_flow();
	void take_optical_flow_frame();
	//tmp
	int write_flow(float *u, float *v, int w, int h);
};


#endif /* GEODESIC_DISTANCE_UI_H_ */
