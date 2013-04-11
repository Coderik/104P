/*
 * patch_weight_ui_container.cpp
 *
 *  Created on: Apr 9, 2013
 *      Author: Vadim Fedorov
 */

#include "patch_weight_ui_container.h"


Patch_Weight_UI_Container::Patch_Weight_UI_Container()
{
	build_ui();
}


Patch_Weight_UI_Container::~Patch_Weight_UI_Container()
{
	// TODO: destroy ui here
}


void Patch_Weight_UI_Container::attach_ui(Gtk::Box *placeholder)
{
	placeholder->pack_start(*_patch_view_frame, Gtk::PACK_SHRINK);
	placeholder->pack_start(*_parameters_frame, Gtk::PACK_SHRINK);
	placeholder->pack_start(*_coordinates_frame, Gtk::PACK_EXPAND_WIDGET);
}


/* Private */


void Patch_Weight_UI_Container::build_ui()
{
	// Build layout.
	_patch_view_frame = new Gtk::Frame("Patch view");
	_patch_view_frame->set_label_align(Gtk::ALIGN_CENTER, Gtk::ALIGN_START);
	_patch_view_frame->set_shadow_type(Gtk::SHADOW_ETCHED_OUT);
	_patch_view_frame->set_size_request(200,200);
	_patch_view_frame->set_margin_bottom(10);

	Gtk::Box *patch_view_box = new Gtk::VBox();			// containes patch view itself and zoom slider
	_patch_view_frame->add(*patch_view_box);

	_parameters_frame = new Gtk::Frame("Parameters");
	_parameters_frame->set_label_align(Gtk::ALIGN_CENTER, Gtk::ALIGN_START);
	_parameters_frame->set_shadow_type(Gtk::SHADOW_ETCHED_OUT);
	_parameters_frame->set_margin_bottom(10);

	Gtk::Box *parameters_box = new Gtk::VBox();			// containes parameters adjustment controls
	_parameters_frame->add(*parameters_box);

	_coordinates_frame = new Gtk::Frame("Coordinates");
	_coordinates_frame->set_label_align(Gtk::ALIGN_CENTER, Gtk::ALIGN_START);
	_coordinates_frame->set_shadow_type(Gtk::SHADOW_ETCHED_OUT);

	// Fill layout with controls.
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
	_coordinates_frame->add(*coordinates_label);
}


