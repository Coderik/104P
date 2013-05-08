/*
 * hull.h
 *
 *  Created on: Jan 9, 2013
 *      Author: Vadim Fedorov
 */

#ifndef HULL_H_
#define HULL_H_

#include <gtkmm/main.h>
#include <gtkmm/window.h>
#include <glibmm/ustring.h>
#include <gtkmm/filechooserdialog.h>
#include <giomm/file.h>
#include <glibmm/dispatcher.h>
#include <gtkmm/radioaction.h>
#include <glibmm/threads.h>
#include <gdk/gdkkeysyms.h>
#include <gtkmm/recentmanager.h>

#include <string.h>
#include <iostream>
#include <algorithm>

#include "i_hull.h"
#include "rig.h"
#include "i_rig_manager.h"
#include "fitting.h"
#include "ui_container.h"
#include "image.h"
#include "sequence.h"
#include "pgm_util.h"
#include "zach_TVL1_optical_flow.h"
#include "util.h"
#include "optical_flow_io.h"
#include "optical_flow_io_legacy.h"
#include "optical_flow.h"
#include "optical_flow_container.h"
#include "signal_watchdog.h"
#include "point.h"
#include "shape.h"
#include "layer_manager.h"

using namespace std;

class Hull : public Gtk::Window, public IHull, public I_Rig_Manager
{
public:
	Hull();
	virtual ~Hull();

	virtual void add_rig(Rig* rig, std::string display_name);
	virtual void initialize_rigs();

	virtual Sequence<float>* request_sequence();
	virtual vector<OpticalFlowContainer*> request_forward_optical_flow();
	virtual vector<OpticalFlowContainer*> request_backward_optical_flow();
	virtual bool request_has_optical_flow_data();
	virtual Layer_Manager* request_layer_manager();
	virtual Gtk::Box* request_ui_placeholder();
	virtual int request_current_time();

protected:
	/* slots */
	void open_image();
	void open_sequence();
	void open_recent();
	void left_button_pressed(int mouse_x, int mouse_y);
	void left_button_released(int mouse_x, int mouse_y);
	void left_button_drag(int mouse_x, int mouse_y);
	void set_time();
	void restore_optical_flow();
	void update_view();
	void update_fitting();
	void set_layers_visibility();
	void perceive_background_worker(int responce_id);	//TODO: rename it!
	bool allow_background_computation();

	void begin_full_optical_flow_calculation();
	void begin_missing_optical_flow_calculation();

	bool key_pressed(GdkEventKey* event);

private:
	vector<Fitting* > _fittings;
	Fitting *_current_fitting;
	Sequence<float> *_sequence;
	Glib::RefPtr<Gdk::Pixbuf> _optical_flow_view;
	std::vector<OpticalFlowContainer*> _forward_optical_flow_list;
	std::vector<OpticalFlowContainer*> _backward_optical_flow_list;
	std::vector<int> _task_list;
	std::string _sequence_folder;
	int _current_time;
	int _progress_counter, _progress_total;
	bool _has_optical_flow_data;
	bool _optical_flow_legacy_format;
	bool _layers_visibility;
	Glib::Threads::Thread *_background_worker;
	Glib::Dispatcher _work_done_dispatcher;
	Glib::Dispatcher _portion_ready_dispatcher;

	Glib::Threads::Mutex _background_work_mutex;	// covers following variables
	bool _aux_stop_background_work_flag;
	float* _aux_optical_flow_x;
	float* _aux_optical_flow_y;
	int _aux_optical_flow_index;

	UI_Container _ui;

	void load_sequence(string path);
	void load_image(string filename);

	template <typename T>
	void reset_vector_of_pointers(std::vector<T*> &v, int size);

	Glib::RefPtr<Gdk::Pixbuf> wrap_raw_image_data(Image<float> *image);
	void update_image_control(int current_time);
	Glib::RefPtr<Gdk::Pixbuf> create_empty_pixbuf(int width, int height);
	void show_status_message(std::string message);

	void store_optical_flow(OpticalFlowContainer &flow, int index);
	void calculate_optical_flow(std::vector<int> task_list);
	void begin_optical_flow_calculation_internal(std::vector<int> task_list);
	void end_calculate_optical_flow();
	void cancel_calculate_optical_flow();
	void take_optical_flow_frame();
	void fill_task_list(std::vector<OpticalFlowContainer*> &forward_flow, std::vector<OpticalFlowContainer*> &backward_flow, std::vector<int> &task_list);
	//tmp
	int write_flow(float *u, float *v, int w, int h);
};


#endif /* HULL_H_ */
