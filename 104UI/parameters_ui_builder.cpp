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

#include "parameters_ui_builder.h"


ParametersUIBuilder::ParametersUIBuilder(IParameterizable *parameterizable)
{
	_ui = 0;

	if (parameterizable) {
        _parameters = parameterizable->get_parameters();
    }
}


void ParametersUIBuilder::add_parameter(IParameter *parameter)
{
    _parameters.push_back(parameter);
}

Gtk::VBox* ParametersUIBuilder::get_ui()
{
	if (!_ui) {
		_ui = build_ui();
	}

	return _ui;
}


/* Private */

Gtk::VBox* ParametersUIBuilder::build_ui()
{
	Gtk::VBox *parameters_box = new Gtk::VBox();

	if (_parameters.size() == 0) {
		Gtk::Label *no_parameters_label = new Gtk::Label("No parameters to configure");
		parameters_box->pack_start(*no_parameters_label, Gtk::PACK_SHRINK);
		return parameters_box;
	}

	for (auto it = _parameters.begin(); it != _parameters.end(); ++it) {
		if (IRangeParameter *parameter = dynamic_cast<IRangeParameter*>(*it)) {
			// create new slider for range parameter
			Gtk::Frame *frame = new Gtk::Frame(parameter->get_display_name());
			frame->set_shadow_type(Gtk::SHADOW_NONE);
			Gtk::Box *slider_box = new Gtk::HBox();
			Gtk::Scale *slider = new Gtk::Scale(Gtk::ORIENTATION_HORIZONTAL);
			slider->set_draw_value(false);
			NumericalEntry *slider_input = new NumericalEntry();
			slider_box->pack_start(*slider_input, Gtk::PACK_SHRINK);
			slider_box->pack_start(*slider, Gtk::PACK_EXPAND_WIDGET);
			frame->add(*slider_box);
			parameters_box->pack_start(*frame, Gtk::PACK_SHRINK);

			// initialize slider
			slider->set_range(parameter->get_lower_boundary(), parameter->get_upper_boundary());
			slider->set_digits(parameter->get_digits());
			slider->set_value(parameter->get());

			slider_input->set_digits(parameter->get_digits());
			slider_input->set_adjustment(slider->get_adjustment());
			slider_input->set_value(parameter->get());

			slider->signal_value_changed().connect( sigc::bind<Gtk::Scale*, IRangeParameter* >( sigc::mem_fun(*this, &ParametersUIBuilder::set_range_parameter), slider, parameter) );

		} else if (IListParameter *parameter = dynamic_cast<IListParameter*>(*it)) {
			// create new combo box for list parameter
			Gtk::Frame *frame = new Gtk::Frame(parameter->get_display_name());
			frame->set_shadow_type(Gtk::SHADOW_NONE);
			Gtk::ComboBoxText *combo_box = new Gtk::ComboBoxText();
			frame->add(*combo_box);
			parameters_box->pack_start(*frame, Gtk::PACK_SHRINK);

			// initialize combo box
			std::vector<std::string> items = parameter->get_items();
			for (auto it = items.begin(); it != items.end(); ++it) {
				combo_box->append(*it);
			}
			combo_box->set_active(parameter->get());

			combo_box->signal_changed().connect( sigc::bind<Gtk::ComboBoxText*, IListParameter* >( sigc::mem_fun(*this, &ParametersUIBuilder::set_list_parameter), combo_box, parameter) );
		}
	}

	return parameters_box;
}


void ParametersUIBuilder::set_range_parameter(Gtk::Scale *slider, IRangeParameter *parameter)
{
	float value = slider->get_value();
	parameter->set(value);
	_signal_changed.emit();
}

void ParametersUIBuilder::set_list_parameter(Gtk::ComboBoxText *combo_box, IListParameter *parameter)
{
	int id = combo_box->get_active_row_number();
	parameter->set(id);
	_signal_changed.emit();
}
