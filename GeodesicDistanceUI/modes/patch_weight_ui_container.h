/*
 * patch_weight_ui_container.h
 *
 *  Created on: Apr 9, 2013
 *      Author: Vadim Fedorov
 */

#ifndef PATCH_WEIGHT_UI_CONTAINER_H_
#define PATCH_WEIGHT_UI_CONTAINER_H_

#include <gtkmm/scale.h>
#include <gtkmm/comboboxtext.h>
#include <gtkmm/label.h>

class Patch_Weight_UI_Container
{
public:
	Gtk::Scale *patch_zoom_slider;
	Gtk::ComboBoxText *distance_mode_picker;
	Gtk::ComboBoxText *motion_compensation_picker;
	Gtk::ComboBoxText *patch_size_picker;
	Gtk::ComboBoxText *patch_duration_picker;
	Gtk::Scale *distance_weight_slider;
	Gtk::Scale *color_weight_slider;
	Gtk::Scale *gamma_slider;
	Gtk::Label *coordinates_label;

private:
}


#endif /* PATCH_WEIGHT_UI_CONTAINER_H_ */
