/*
 * ui_container.h
 *
 *  Created on: Jan 9, 2013
 *      Author: Vadim Fedorov
 */

#include <string>
#include <vector>
#include <map>
#include <gtkmm/widget.h>
#include <gtkmm/window.h>
#include <gtkmm/stock.h>
#include <gtkmm/uimanager.h>
#include <gtkmm/box.h>
#include <gtkmm/frame.h>
#include <gtkmm/scale.h>
#include <gtkmm/comboboxtext.h>
#include <gtkmm/statusbar.h>
#include <gtkmm/label.h>
#include <gtkmm/recentaction.h>
#include <gtkmm/recentfilter.h>
#include <gtkmm/recentmanager.h>
#include <gtkmm/toggleaction.h>
#include <gtkmm/radioaction.h>
#include <gtkmm/infobar.h>
#include <sigc++/sigc++.h>
#include <gtkmm/menubar.h>
#include <glibmm/convert.h>

#include "image_viewer.h"
#include "layer_manager.h"
#include "fitting.h"

using namespace std;

class UI_Container
{
public:
	Glib::RefPtr<Gtk::Action> open_image_action;
	Glib::RefPtr<Gtk::Action> open_sequence_action;
	Glib::RefPtr<Gtk::RecentAction> open_recent_action;
	Glib::RefPtr<Gtk::Action> quit_action;
	Glib::RefPtr<Gtk::ActionGroup> optical_flow_action_group;
	Glib::RefPtr<Gtk::Action> calculate_optical_flow_action;
	Glib::RefPtr<Gtk::Action> proceed_optical_flow_action;
	Glib::RefPtr<Gtk::Action> restore_optical_flow_action;
	Glib::RefPtr<Gtk::ActionGroup> view_action_group;
	Glib::RefPtr<Gtk::ActionGroup> fitting_action_group;
	Glib::RefPtr<Gtk::ActionGroup> layer_action_group;
	Glib::RefPtr<Gtk::ToggleAction> layers_visibility_toggle_action;

	Gtk::Box *left_side_layout;
	Gtk::Box *right_side_layout;
	ImageViewer *image_control;
	Gtk::Scale *time_slider;
	Gtk::Statusbar *status_bar;
	Gtk::InfoBar *background_work_infobar;
	Gtk::Label *background_work_infobar_message;

	typedef sigc::signal<void> type_signal_view_changed;
	type_signal_view_changed signal_view_changed()
	{
		return _signal_view_changed;
	}

	typedef sigc::signal<void> type_signal_fitting_changed;
	type_signal_fitting_changed signal_fitting_changed()
	{
		return _signal_fitting_changed;
	}

	enum View
	{
		VIEW_ORIGINAL_IMAGE,
		VIEW_FORWARD_OF_COLOR,
		VIEW_FORWARD_OF_GRAY,
		VIEW_BACKWARD_OF_COLOR,
		VIEW_BACKWARD_OF_GRAY,
	};

	void setup_ui(Gtk::Window* window, string application_id);

	void add_recent_file(string filename);
	void add_recent_folder(string path);

	void refresh_placeholders();
	void clear_placeholders();

	void assign_menu(Gtk::Menu *menu, string title);

	View get_view();
	/*
	 * NOTE: Does not emit 'signal_view_changed'.
	 */
	void set_view(View view);
	void allow_optical_flow_views(bool is_allowed);

	void set_fittings(std::vector<Fitting* > fittings);
	Fitting* get_fitting();

private:
	string _application_id;
	Glib::RefPtr<Gtk::UIManager> _menu_manager;
	Gtk::MenuBar *_menu_bar;
	int _fitting_submenu_merge_id;
	std::map<View, Glib::RefPtr<Gtk::RadioAction> > _view_map;
	std::map<Glib::ustring, Fitting* > _fitting_map;
	type_signal_view_changed _signal_view_changed;
	type_signal_fitting_changed _signal_fitting_changed;
	View _current_view;
	Fitting *_current_fitting;


	void add_recent_document_internal(string path, string mime_type);

	void set_view_internal(const Glib::RefPtr<Gtk::RadioAction>& current);

	void set_fitting_internal(const Glib::RefPtr<Gtk::RadioAction>& current);

};



