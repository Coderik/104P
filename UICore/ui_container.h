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
#include <gtkmm/toggleaction.h>
#include <gtkmm/radioaction.h>
#include <gtkmm/infobar.h>
#include <sigc++/sigc++.h>

#include "selectable_image.h"
#include "layer_manager.h"
#include "fitting.h"

using namespace std;

class UI_Container
{
public:
	Glib::RefPtr<Gtk::Action> open_image_action;
	Glib::RefPtr<Gtk::Action> open_sequence_action;
	Glib::RefPtr<Gtk::Action> quit_action;
	Glib::RefPtr<Gtk::ActionGroup> optical_flow_action_group;
	Glib::RefPtr<Gtk::Action> calculate_optical_flow_action;
	Glib::RefPtr<Gtk::Action> proceed_optical_flow_action;
	Glib::RefPtr<Gtk::Action> restore_optical_flow_action;
	Glib::RefPtr<Gtk::ActionGroup> view_action_group;
	Glib::RefPtr<Gtk::ActionGroup> fitting_action_group;
	Glib::RefPtr<Gtk::ActionGroup> layer_action_group;
	Glib::RefPtr<Gtk::ToggleAction> layers_visibility_toggle_action;

	Gtk::Box *right_side_layout;
	Selectable_Image *image_control;
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

	void setup_ui(Gtk::Window* window)
	{
		_current_view = VIEW_ORIGINAL_IMAGE;

		// adjust main window
		window->set_title("Geodesic Distance test app");
		window->set_default_size(800, 700);

		// set up menu
		_menu_manager = Gtk::UIManager::create();

		Glib::RefPtr<Gtk::ActionGroup> action_group = Gtk::ActionGroup::create();
		action_group->add(Gtk::Action::create("FileMenu", "File"));
		open_image_action = Gtk::Action::create("OpenImage", "Open Image File...");
		action_group->add(open_image_action);
		open_sequence_action = Gtk::Action::create("OpenSequence", "Open Sequence Folder...");
		action_group->add(open_sequence_action);
		quit_action = Gtk::Action::create("Quit", Gtk::Stock::QUIT);
		action_group->add(quit_action);
		_menu_manager->insert_action_group(action_group);

		optical_flow_action_group = Gtk::ActionGroup::create();
		optical_flow_action_group->add(Gtk::Action::create("OpticalFlowMenu", "Optical Flow"));
		calculate_optical_flow_action = Gtk::Action::create("CalculateOptFlow", "Calculate Optical Flow");
		optical_flow_action_group->add(calculate_optical_flow_action);
		proceed_optical_flow_action = Gtk::Action::create("ProceedOptFlow", "Proceed Calculaton");
		optical_flow_action_group->add(proceed_optical_flow_action);
		restore_optical_flow_action = Gtk::Action::create("RestoreOptFlow", "Restore Optical Flow");
		optical_flow_action_group->add(restore_optical_flow_action);
		_menu_manager->insert_action_group(optical_flow_action_group);

		view_action_group = Gtk::ActionGroup::create();
		view_action_group->add(Gtk::Action::create("ViewMenu", "View"));
		Gtk::RadioAction::Group view_group = Gtk::RadioAction::Group();
		Glib::RefPtr<Gtk::RadioAction> view_action = Gtk::RadioAction::create(view_group, "ImageView", "Original Image");
		view_action_group->add(view_action);
		_view_map[VIEW_ORIGINAL_IMAGE] = view_action;
		view_action = Gtk::RadioAction::create(view_group, "ForwardOFColorView", "Forward Optical Flow (direction and magnitude)");
		view_action_group->add(view_action);
		_view_map[VIEW_FORWARD_OF_COLOR] = view_action;
		view_action = Gtk::RadioAction::create(view_group, "ForwardOFGrayView", "Forward Optical Flow (magnitude)");
		view_action_group->add(view_action);
		_view_map[VIEW_FORWARD_OF_GRAY] = view_action;
		view_action = Gtk::RadioAction::create(view_group, "BackwardOFColorView", "Backward Optical Flow (direction and magnitude)");
		view_action_group->add(view_action);
		_view_map[VIEW_BACKWARD_OF_COLOR] = view_action;
		view_action = Gtk::RadioAction::create(view_group, "BackwardOFGrayView", "Backward Optical Flow (magnitude)");
		view_action_group->add(view_action);
		_view_map[VIEW_BACKWARD_OF_GRAY] = view_action;
		_menu_manager->insert_action_group(view_action_group);
		view_action->signal_changed().connect( sigc::mem_fun(*this, &UI_Container::set_view_internal) );

		fitting_action_group = Gtk::ActionGroup::create();
		fitting_action_group->add(Gtk::Action::create("FittingMenu", "Fitting"));
		_menu_manager->insert_action_group(fitting_action_group);

		layer_action_group = Gtk::ActionGroup::create();
		layer_action_group->add(Gtk::Action::create("LayerMenu", "Layers"));
		layers_visibility_toggle_action = Gtk::ToggleAction::create("LayersVisibility", "Show all layers");
		layer_action_group->add(layers_visibility_toggle_action);
		_menu_manager->insert_action_group(layer_action_group);
		layer_action_group->set_sensitive(false);

		Glib::ustring ui_info =
		    "<ui>"
		    "  <menubar name='MenuBar'>"
		    "    <menu action='FileMenu'>"
		    "      <menuitem action='OpenImage'/>"
		    "      <menuitem action='OpenSequence'/>"
		    "      <separator/>"
		    "      <menuitem action='Quit'/>"
		    "    </menu>"
			"    <menu action='OpticalFlowMenu'>"
			"      <menuitem action='RestoreOptFlow'/>"
			"      <separator/>"
			"      <menuitem action='CalculateOptFlow'/>"
			"      <menuitem action='ProceedOptFlow'/>"
			"    </menu>"
			"    <menu action='ViewMenu'>"
			"      <menuitem action='ImageView'/>"
			"      <menuitem action='ForwardOFColorView'/>"
			"      <menuitem action='ForwardOFGrayView'/>"
			"      <menuitem action='BackwardOFColorView'/>"
			"      <menuitem action='BackwardOFGrayView'/>"
			"    </menu>"
			"    <menu action='LayerMenu'>"
			"      <menuitem action='LayersVisibility'/>"
			"    </menu>"
			"    <menu action='FittingMenu'>"
			"    </menu>"
		    "  </menubar>"
		    "</ui>";
		_menu_manager->add_ui_from_string(ui_info);

		// create layout
		Gtk::Box *window_layout = new Gtk::VBox();		// containes menubar, infobar, main working area and statusbar
		window->add(*window_layout);

		Gtk::Widget* menu_bar = _menu_manager->get_widget("/MenuBar");
		if (menu_bar) {
			window_layout->pack_start(*menu_bar, Gtk::PACK_SHRINK);
		}

		background_work_infobar = new Gtk::InfoBar();
		background_work_infobar->set_message_type(Gtk::MESSAGE_INFO);
		window_layout->pack_start(*background_work_infobar, Gtk::PACK_SHRINK);

		status_bar = new Gtk::Statusbar();
		window_layout->pack_end(*status_bar,  Gtk::PACK_SHRINK);

		Gtk::Box *working_area_layout = new Gtk::HBox();	// containes right and left 'sides'
		working_area_layout->set_margin_top(10);
		working_area_layout->set_margin_left(10);
		working_area_layout->set_margin_right(10);
		window_layout->pack_start(*working_area_layout, Gtk::PACK_EXPAND_WIDGET);

		Gtk::Box *left_side_layout = new Gtk::VBox();		// containes image/sequence view and time scale
		left_side_layout->set_margin_right(10);
		working_area_layout->pack_start(*left_side_layout, Gtk::PACK_EXPAND_WIDGET);

		Gtk::Frame *image_view_frame = new Gtk::Frame("Image/sequence view");
		image_view_frame->set_label_align(Gtk::ALIGN_CENTER, Gtk::ALIGN_START);
		image_view_frame->set_shadow_type(Gtk::SHADOW_ETCHED_OUT);
		left_side_layout->pack_start(*image_view_frame, Gtk::PACK_EXPAND_WIDGET);

		time_slider = new Gtk::Scale(Gtk::ORIENTATION_HORIZONTAL);
		left_side_layout->pack_start(*time_slider, Gtk::PACK_SHRINK);

		right_side_layout = new Gtk::VBox();		// rig placeholder
		working_area_layout->pack_start(*right_side_layout, Gtk::PACK_SHRINK);

		// fill layout with controls
		background_work_infobar->add_button(Gtk::Stock::CANCEL, 0);
		Gtk::Container* infobar_container =
					dynamic_cast<Gtk::Container*>(background_work_infobar->get_content_area());
		if (infobar_container) {
			Gtk::HBox *infobar_box = new Gtk::HBox();
			infobar_container->add(*infobar_box);
			Gtk::Label *infobar_caption = new Gtk::Label("Background calculations. ");
			infobar_box->pack_start(*infobar_caption, Gtk::PACK_SHRINK);
			background_work_infobar_message = new Gtk::Label();
			infobar_box->pack_start(*background_work_infobar_message, Gtk::PACK_EXPAND_PADDING);
		}

		image_control = new Selectable_Image();
		image_view_frame->add(*image_control);

		// show all
		window->show_all_children();
	}


	void refresh_placeholders()
	{
		right_side_layout->show_all_children(true);
	}


	void clear_placeholders()
	{
		vector<Gtk::Widget* > children = right_side_layout->get_children();
		vector<Gtk::Widget* >::iterator it;
		for(it = children.begin(); it != children.end(); ++it) {
			right_side_layout->remove(**it);
		}
	}


	View get_view()
	{
		return _current_view;
	}


	/*
	 * NOTE: Does not emit 'signal_view_changed'.
	 */
	void set_view(View view)
	{
		if (_current_view != view) {
			_current_view = view;
			_view_map[_current_view]->set_active(true);
		}
	}


	void allow_optical_flow_views(bool is_allowed)
	{
		// NOTE: This must be generalized when there would be more categories of views
		_view_map[VIEW_FORWARD_OF_COLOR]->set_sensitive(is_allowed);
		_view_map[VIEW_FORWARD_OF_GRAY]->set_sensitive(is_allowed);
		_view_map[VIEW_BACKWARD_OF_COLOR]->set_sensitive(is_allowed);
		_view_map[VIEW_BACKWARD_OF_GRAY]->set_sensitive(is_allowed);
	}

	void set_fittings(std::vector<Fitting* > fittings)
	{
		// TODO: handle multiple call (do nothing or reinitialize)

		if (fittings.size() == 0) {
			return;
		}

		Glib::ustring ui_info =
			"<ui>"
			"  <menubar name='MenuBar'>"
			"    <menu action='FittingMenu'>";


		Gtk::RadioAction::Group group = Gtk::RadioAction::Group();
		Glib::RefPtr<Gtk::RadioAction> action;
		std::string name;
		for (int i = 0; i < fittings.size(); i++) {
			// int to string
			std::stringstream s;
			s << i;
			s >> name;

			_fitting_map[name] = fittings[i];
			action = Gtk::RadioAction::create(group, name, fittings[i]->display_name);
			ui_info += Glib::ustring::compose ("      <menuitem action='%1'/>", name);
			fitting_action_group->add(action);
		}

		ui_info +=
			"    </menu>"
			"  </menubar>"
			"</ui>";

		_fitting_submenu_merge_id = _menu_manager->add_ui_from_string (ui_info);
		//_menu_manager->insert_action_group(fitting_action_group);

		action->signal_changed().connect( sigc::mem_fun(*this, &UI_Container::set_fitting_internal) );

		_current_fitting = fittings[0];
		_signal_fitting_changed.emit();
	}

	Fitting* get_fitting()
	{
		return _current_fitting;
	}

private:
	Glib::RefPtr<Gtk::UIManager> _menu_manager;
	int _fitting_submenu_merge_id;
	std::map<View, Glib::RefPtr<Gtk::RadioAction> > _view_map;
	std::map<Glib::ustring, Fitting* > _fitting_map;
	type_signal_view_changed _signal_view_changed;
	type_signal_fitting_changed _signal_fitting_changed;
	View _current_view;
	Fitting *_current_fitting;

	void set_view_internal(const Glib::RefPtr<Gtk::RadioAction>& current)
	{
		Glib::ustring name = current->get_name();
		if (name == "ImageView") {
			_current_view = VIEW_ORIGINAL_IMAGE;
		} else if (name == "ForwardOFColorView") {
			_current_view = VIEW_FORWARD_OF_COLOR;
		} else if (name == "ForwardOFGrayView") {
			_current_view = VIEW_FORWARD_OF_GRAY;
		} else if (name == "BackwardOFColorView") {
			_current_view = VIEW_BACKWARD_OF_COLOR;
		} else if (name == "BackwardOFGrayView") {
			_current_view = VIEW_BACKWARD_OF_GRAY;
		} else {
			_current_view = VIEW_ORIGINAL_IMAGE;
		}

		_signal_view_changed.emit();
	}

	void set_fitting_internal(const Glib::RefPtr<Gtk::RadioAction>& current)
	{
		Glib::ustring name = current->get_name();

		_current_fitting = _fitting_map[name];

		_signal_fitting_changed.emit();
	}

};




