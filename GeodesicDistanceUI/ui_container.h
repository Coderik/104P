/*
 * ui_container.h
 *
 *  Created on: Jan 9, 2013
 *      Author: Vadim Fedorov
 */

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
#include <map>

#include "selectable_image.h"
#include "layer_manager.h"


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
	Glib::RefPtr<Gtk::ActionGroup> layer_action_group;
	Glib::RefPtr<Gtk::ToggleAction> layers_visibility_toggle_action;

	Selectable_Image *image_control;
	Selectable_Image *patch_control;
	Gtk::Scale *patch_zoom_slider;
	Gtk::ComboBoxText *distance_mode_picker;
	Gtk::ComboBoxText *motion_compensation_picker;
	Gtk::ComboBoxText *patch_size_picker;
	Gtk::ComboBoxText *patch_duration_picker;
	Gtk::Scale *distance_weight_slider;
	Gtk::Scale *color_weight_slider;
	Gtk::Scale *gamma_slider;
	Gtk::Scale *time_slider;
	Gtk::Label *coordinates_label;
	Gtk::Statusbar *status_bar;
	Gtk::InfoBar *background_work_infobar;
	Gtk::Label *background_work_infobar_message;

	typedef sigc::signal<void> type_signal_view_changed;
	type_signal_view_changed signal_view_changed()
	{
		return _signal_view_changed;
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
		Glib::RefPtr<Gtk::UIManager> menu_manager = Gtk::UIManager::create();

		Glib::RefPtr<Gtk::ActionGroup> action_group = Gtk::ActionGroup::create();
		action_group->add(Gtk::Action::create("FileMenu", "File"));
		open_image_action = Gtk::Action::create("OpenImage", "Open Image File...");
		action_group->add(open_image_action);
		open_sequence_action = Gtk::Action::create("OpenSequence", "Open Sequence Folder...");
		action_group->add(open_sequence_action);
		quit_action = Gtk::Action::create("Quit", Gtk::Stock::QUIT);
		action_group->add(quit_action);
		menu_manager->insert_action_group(action_group);

		optical_flow_action_group = Gtk::ActionGroup::create();
		optical_flow_action_group->add(Gtk::Action::create("OpticalFlowMenu", "Optical Flow"));
		calculate_optical_flow_action = Gtk::Action::create("CalculateOptFlow", "Calculate Optical Flow");
		optical_flow_action_group->add(calculate_optical_flow_action);
		proceed_optical_flow_action = Gtk::Action::create("ProceedOptFlow", "Proceed Calculaton");
		optical_flow_action_group->add(proceed_optical_flow_action);
		restore_optical_flow_action = Gtk::Action::create("RestoreOptFlow", "Restore Optical Flow");
		optical_flow_action_group->add(restore_optical_flow_action);
		menu_manager->insert_action_group(optical_flow_action_group);

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
		menu_manager->insert_action_group(view_action_group);
		view_action->signal_changed().connect( sigc::mem_fun(*this, &UI_Container::set_view_internal) );

		layer_action_group = Gtk::ActionGroup::create();
		layer_action_group->add(Gtk::Action::create("LayerMenu", "Layers"));
		layers_visibility_toggle_action = Gtk::ToggleAction::create("LayersVisibility", "Patch Position");
		layer_action_group->add(layers_visibility_toggle_action);
		menu_manager->insert_action_group(layer_action_group);
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
		    "  </menubar>"
		    "</ui>";
		menu_manager->add_ui_from_string(ui_info);

		// create layout
		Gtk::Box *window_layout = new Gtk::VBox();		// containes menubar, infobar, main working area and statusbar
		window->add(*window_layout);

		Gtk::Widget* menu_bar = menu_manager->get_widget("/MenuBar");
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

		Gtk::Box *right_side_layout = new Gtk::VBox();		// containes patch view, params panel and coordinates
		working_area_layout->pack_start(*right_side_layout, Gtk::PACK_SHRINK);

		Gtk::Frame *patch_view_frame = new Gtk::Frame("Patch view");
		patch_view_frame->set_label_align(Gtk::ALIGN_CENTER, Gtk::ALIGN_START);
		patch_view_frame->set_shadow_type(Gtk::SHADOW_ETCHED_OUT);
		patch_view_frame->set_size_request(200,200);
		patch_view_frame->set_margin_bottom(10);
		right_side_layout->pack_start(*patch_view_frame, Gtk::PACK_SHRINK);

		Gtk::Box *patch_view_box = new Gtk::VBox();			// containes patch view itself and zoom slider
		patch_view_frame->add(*patch_view_box);

		Gtk::Frame *parameters_frame = new Gtk::Frame("Parameters");
		parameters_frame->set_label_align(Gtk::ALIGN_CENTER, Gtk::ALIGN_START);
		parameters_frame->set_shadow_type(Gtk::SHADOW_ETCHED_OUT);
		parameters_frame->set_margin_bottom(10);
		right_side_layout->pack_start(*parameters_frame, Gtk::PACK_SHRINK);

		Gtk::Box *parameters_box = new Gtk::VBox();			// containes parameters adjustment controls
		parameters_frame->add(*parameters_box);

		Gtk::Frame *coordinates_frame = new Gtk::Frame("Coordinates");
		coordinates_frame->set_label_align(Gtk::ALIGN_CENTER, Gtk::ALIGN_START);
		coordinates_frame->set_shadow_type(Gtk::SHADOW_ETCHED_OUT);
		right_side_layout->pack_start(*coordinates_frame, Gtk::PACK_EXPAND_WIDGET);

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

		patch_control = new Selectable_Image();
		patch_view_box->pack_start(*patch_control, Gtk::PACK_EXPAND_WIDGET);

		patch_zoom_slider = new Gtk::Scale(Gtk::ORIENTATION_HORIZONTAL);
		//patch_zoom_slider->set_draw_value(false);
		patch_view_box->pack_start(*patch_zoom_slider, Gtk::PACK_SHRINK);

		Gtk::Frame *distance_mode_frame = new Gtk::Frame("Distance calc. range:");
		distance_mode_frame->set_shadow_type(Gtk::SHADOW_NONE);
		distance_mode_picker = new Gtk::ComboBoxText();
		distance_mode_frame->add(*distance_mode_picker);
		parameters_box->pack_start(*distance_mode_frame, Gtk::PACK_SHRINK);

		Gtk::Frame *motion_compensation_frame = new Gtk::Frame("Motion tracking");
		motion_compensation_frame->set_shadow_type(Gtk::SHADOW_NONE);
		motion_compensation_picker = new Gtk::ComboBoxText();
		motion_compensation_frame->add(*motion_compensation_picker);
		parameters_box->pack_start(*motion_compensation_frame, Gtk::PACK_SHRINK);

		Gtk::Frame *patch_size_frame = new Gtk::Frame("Patch size:");
		patch_size_frame->set_shadow_type(Gtk::SHADOW_NONE);
		patch_size_picker = new Gtk::ComboBoxText();
		patch_size_frame->add(*patch_size_picker);
		parameters_box->pack_start(*patch_size_frame, Gtk::PACK_SHRINK);

		Gtk::Frame *patch_duration_frame = new Gtk::Frame("Patch duration:");
		patch_duration_frame->set_shadow_type(Gtk::SHADOW_NONE);
		patch_duration_picker = new Gtk::ComboBoxText();
		patch_duration_frame->add(*patch_duration_picker);
		parameters_box->pack_start(*patch_duration_frame, Gtk::PACK_SHRINK);

		Gtk::Frame *distance_weight_frame = new Gtk::Frame("Distance weight:");
		distance_weight_frame->set_shadow_type(Gtk::SHADOW_NONE);
		distance_weight_slider = new Gtk::Scale(Gtk::ORIENTATION_HORIZONTAL);
		distance_weight_slider->set_value_pos(Gtk::POS_LEFT);
		distance_weight_frame->add(*distance_weight_slider);
		parameters_box->pack_start(*distance_weight_frame, Gtk::PACK_SHRINK);

		Gtk::Frame *color_weight_frame = new Gtk::Frame("Color weight:");
		color_weight_frame->set_shadow_type(Gtk::SHADOW_NONE);
		color_weight_slider = new Gtk::Scale(Gtk::ORIENTATION_HORIZONTAL);
		color_weight_slider->set_value_pos(Gtk::POS_LEFT);
		color_weight_frame->add(*color_weight_slider);
		parameters_box->pack_start(*color_weight_frame, Gtk::PACK_SHRINK);

		Gtk::Frame *gamma_frame = new Gtk::Frame("Gamma:");
		gamma_frame->set_shadow_type(Gtk::SHADOW_NONE);
		gamma_slider = new Gtk::Scale(Gtk::ORIENTATION_HORIZONTAL);
		gamma_slider->set_value_pos(Gtk::POS_LEFT);
		gamma_frame->add(*gamma_slider);
		parameters_box->pack_start(*gamma_frame, Gtk::PACK_SHRINK);

		coordinates_label = new Gtk::Label();
		coordinates_frame->add(*coordinates_label);

		// show all
		window->show_all_children();
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

private:
	std::map<View, Glib::RefPtr<Gtk::RadioAction> > _view_map;
	type_signal_view_changed _signal_view_changed;
	View _current_view;

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
};




