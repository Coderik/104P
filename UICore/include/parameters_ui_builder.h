/*
 * parameters_ui_builder.h
 *
 *  Created on: Sep 11, 2013
 *      Author: Vadim Fedorov
 */

#ifndef PARAMETERS_UI_BUILDER_H_
#define PARAMETERS_UI_BUILDER_H_

#include <vector>
#include <gtkmm/box.h>
#include <gtkmm/frame.h>
#include <gtkmm/comboboxtext.h>
#include <gtkmm/scale.h>
#include <gtkmm/spinbutton.h>
#include <sigc++/sigc++.h>
#include "i_parameterizable.h"
#include "i_range_parameter.h"
#include "i_list_parameter.h"
#include "numerical_entry.h"

using namespace std;

class ParametersUIBuilder
{
public:
	ParametersUIBuilder(IParameterizable *parameterizable);

	Gtk::VBox* get_ui();

	typedef sigc::signal<void> type_signal_changed;
	type_signal_changed signal_changed()
	{
		return _signal_changed;
	}

private:
	type_signal_changed _signal_changed;
	IParameterizable *_parameterizable;
	Gtk::VBox *_ui;

	Gtk::VBox* build_ui();
	void set_range_parameter(Gtk::Scale *slider, IRangeParameter *parameter);
	void set_list_parameter(Gtk::ComboBoxText *combo_box, IListParameter *parameter);
};


#endif /* PARAMETERS_UI_BUILDER_H_ */
