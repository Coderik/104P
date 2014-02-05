/*
 * hull.h
 *
 *  Created on: Jan 9, 2013
 *      Author: Vadim Fedorov
 */

#ifndef HULL_H_
#define HULL_H_

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32)
#define OS_Windows
#endif

#include <gtkmm/main.h>
#include <gtkmm/window.h>
#include <glibmm/ustring.h>
#include <gtkmm/filechooserdialog.h>
#include <giomm/file.h>
#include <glibmm.h>
#include <gtkmm/radioaction.h>
#include <gdk/gdkkeysyms.h>
#include <gtkmm/recentmanager.h>

#include <string.h>
#include <iostream>
#include <algorithm>

#include "i_hull.h"
#include "hull_proxy.h"
#include "i_modulable.h"
#include "i_module_manager.h"
#include "rig.h"
#include "i_rig_manager.h"
#include "fitting.h"
#include "ui_container.h"
#include "image.h"
#include "sequence.h"
#include "io_utility.h"
#include "zach_TVL1_optical_flow.h"
#include "optical_flow_io.h"
#include "optical_flow_io_legacy.h"
#include "optical_flow.h"
#include "optical_flow_container.h"
#include "signal_watchdog.h"
#include "point.h"
#include "shape.h"
#include "layer_manager.h"
#include "background_worker.h"
#include "optical_flow_data.h"
#include "mouse_event.h"

using namespace std;

class Hull : public Gtk::Window, public IHull, public IRigManager, public IModulable, public IModuleManager
{
public:
	Hull(string application_id);
	virtual ~Hull();

	virtual void add_rig(Rig* rig, std::string display_name);
	virtual void initialize_rigs();

	virtual Sequence<float>* request_sequence();
	virtual vector<OpticalFlowContainer*> request_forward_optical_flow();
	virtual vector<OpticalFlowContainer*> request_backward_optical_flow();
	virtual bool request_has_optical_flow_data();
	virtual LayerManager* request_layer_manager();
	virtual InteractionManager* request_interaction_manager();
	virtual Gtk::Box* request_ui_placeholder();
	virtual int request_current_time();
	virtual void request_module(RequestBase<IModule> &request);

	virtual void add_module(IModule *module);

	virtual sigc::signal<void> signal_sequence_changed();
	virtual void assign_menu(Gtk::Menu *menu, string title);
	virtual string request_open_filename(string dialog_title, Glib::RefPtr<Gtk::FileFilter> filter = Glib::RefPtr<Gtk::FileFilter>());
	virtual void request_active_rig(RequestBase<IRig> &request);

protected:
	/* slots */
	void open_image();
	void open_sequence();
	void open_recent();
	void left_button_pressed(MouseEvent mouse_event);
	void left_button_released(MouseEvent mouse_event);
	void left_button_drag(MouseEvent mouse_event);
	void set_time();
	void restore_optical_flow();
	void update_view();
	void update_fitting();
	void update_toolbar();
	void set_layers_visibility();
	void perceive_background_worker(int responce_id);	//TODO: rename it!

	void begin_full_optical_flow_calculation();
	void begin_missing_optical_flow_calculation();

	bool key_pressed(GdkEventKey* event);

private:
	sigc::signal<void> _signal_sequence_changed;
	vector<IModule* > _modules;
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
	IBackgroundWorker *_background_worker;
	sigc::connection _connection_interaction_manager_signal_ui_updated;

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
	void calculate_optical_flow(IBackgroundInsider *insider, std::vector<int> task_list);
	void begin_optical_flow_calculation_internal(std::vector<int> task_list);
	void end_calculate_optical_flow();
	void cancel_calculate_optical_flow();
	void take_optical_flow_frame(IData *data);
	void fill_task_list(std::vector<OpticalFlowContainer*> &forward_flow, std::vector<OpticalFlowContainer*> &backward_flow, std::vector<int> &task_list);
	//tmp
	int write_flow(float *u, float *v, int w, int h);
};

#endif /* HULL_H_ */
