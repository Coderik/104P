/**
 * Copyright (C) 2016, Vadim Fedorov <coderiks@gmail.com>
 *
 * This program is free software: you can use, modify and/or
 * redistribute it under the terms of the simplified BSD
 * License. You should have received a copy of this license along
 * this program. If not, see
 * <http://www.opensource.org/licenses/bsd-license.html>.
 */

/// Created on: Sep 11, 2013

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

class ParametersUIBuilder
{
public:
	ParametersUIBuilder(IParameterizable *parameterizable = 0); // TODO: eventually remove IParameterizable from params

    void add_parameter(IParameter *parameter);

	Gtk::VBox* get_ui();

	typedef sigc::signal<void> type_signal_changed;
	type_signal_changed signal_changed()
	{
		return _signal_changed;
	}

private:
	type_signal_changed _signal_changed;
	std::vector<IParameter*> _parameters;
	Gtk::VBox *_ui;

	Gtk::VBox* build_ui();
	void set_range_parameter(Gtk::Scale *slider, IRangeParameter *parameter);
	void set_list_parameter(Gtk::ComboBoxText *combo_box, IListParameter *parameter);
};


#endif /* PARAMETERS_UI_BUILDER_H_ */
