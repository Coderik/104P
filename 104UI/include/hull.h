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
#include <map>
#include <cstdio>

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
#include "point.h"
#include "shape.h"
#include "layer_manager.h"
#include "background_worker.h"
#include "mouse_event.h"
#include "view.h"

using namespace std;

class Hull : public Gtk::Window, public IHull, public IRigManager, public IModulable, public IModuleManager
{
public:
	Hull(string application_id);
	virtual ~Hull();

	// IRigManager members:
	virtual void add_rig(Rig* rig, std::string display_name);
	virtual void initialize_rigs();

	// IHull members:
	virtual SequenceFx<float> request_sequence();
	virtual LayerManager* request_layer_manager();
	virtual InteractionManager* request_interaction_manager();
	virtual Gtk::Box* request_ui_placeholder();
	virtual int request_current_time();
	virtual void request_module(RequestBase<IModule> &request);

	// IModuleManager members:
	virtual void add_module(IModule *module);

	// IModulable members:
	virtual sigc::signal<void> signal_sequence_changed();
	virtual void assign_menu(Gtk::Menu *menu, string title);
	virtual string request_sequence_path();
	virtual string request_open_dialog_result(string dialog_title, bool folder = false, Glib::RefPtr<Gtk::FileFilter> filter = Glib::RefPtr<Gtk::FileFilter>());
	virtual string request_save_dialog_result(string dialog_title, Glib::RefPtr<Gtk::FileFilter> filter = Glib::RefPtr<Gtk::FileFilter>());
	virtual void request_active_rig(RequestBase<IRig> &request);
	virtual Descriptor add_view(string title, sigc::slot1<Glib::RefPtr<Gdk::Pixbuf>, int> provider);
	virtual bool alter_view(Descriptor view_descriptor, string title, sigc::slot1<Glib::RefPtr<Gdk::Pixbuf>, int> provider);
	virtual bool remove_view(Descriptor view_descriptor);
	virtual bool queue_view_draw(Descriptor view_descriptor);
	virtual Descriptor add_background_work_info(sigc::slot0<void> cancel_slot, string message = "");
	virtual bool alter_background_work_info(Descriptor descriptor, string message);
	virtual bool remove_background_work_info(Descriptor descriptor);

protected:
	/* slots */
	void open_image();
	void open_sequence();
	void open_recent();
	void left_button_pressed(MouseEvent mouse_event);
	void left_button_released(MouseEvent mouse_event);
	void left_button_drag(MouseEvent mouse_event);
	void set_time();
	void active_view_changed(const Descriptor &active_view);
	void update_fitting();
	void update_toolbar();
	void set_layers_visibility();
	void background_worker_infobar_responded(int responce_id);

	bool key_pressed(GdkEventKey* event);

	Glib::RefPtr<Gdk::Pixbuf> provide_original_image_view(unsigned int time);

private:
	sigc::signal<void> _signal_sequence_changed;
	vector<IModule* > _modules;
	vector<Fitting* > _fittings;
	Fitting *_current_fitting;
	SequenceFx<float> _sequence;
	std::map<Descriptor, View* > _view_map;
	Descriptor _active_view;
	Descriptor _original_image_view;
	Descriptor _background_work_info;
	sigc::slot0<void> _background_work_cancel_slot;
	std::string _sequence_folder;
	int _current_time;
	bool _layers_visibility;
	sigc::connection _connection_interaction_manager_signal_ui_updated;

	UI_Container _ui;

	void load_sequence(string path);
	void load_image(string filename);

	void refresh_view_menu();

	Glib::RefPtr<Gdk::Pixbuf> wrap_raw_image_data(const ImageFx<float> &image);
	void update_image_control(int current_time);
	Glib::RefPtr<Gdk::Pixbuf> create_empty_pixbuf(int width, int height);
	void show_status_message(std::string message);

	//tmp
	int write_flow(float *u, float *v, int w, int h);
};

#endif /* HULL_H_ */
