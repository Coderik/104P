/*
 * parameters_ui_builder.cpp
 *
 *  Created on: Sep 11, 2013
 *      Author: Vadim Fedorov
 */

#include "parameters_ui_builder.h"


ParametersUIBuilder::ParametersUIBuilder(IParameterizable *parameterizable)
{
	_ui = 0;
	_parameterizable = parameterizable;
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
	vector<IParameter*> parameters = _parameterizable->get_parameters();

	if (parameters.size() == 0) {
		Gtk::Label *no_parameters_label = new Gtk::Label("No parameters to configure");
		parameters_box->pack_start(*no_parameters_label, Gtk::PACK_SHRINK);
		return parameters_box;
	}

	vector<IParameter*>::iterator it;
	for (it = parameters.begin(); it != parameters.end(); ++it) {
		if (IRangeParameter *parameter = dynamic_cast<IRangeParameter*>(*it)) {
			// create new slider for range parameter
			Gtk::Frame *frame = new Gtk::Frame(parameter->get_display_name());
			frame->set_shadow_type(Gtk::SHADOW_NONE);
			Gtk::Scale *slider = new Gtk::Scale(Gtk::ORIENTATION_HORIZONTAL);
			slider->set_value_pos(Gtk::POS_LEFT);
			frame->add(*slider);
			parameters_box->pack_start(*frame, Gtk::PACK_SHRINK);

			// initialize slider
			slider->set_range(parameter->get_lower_boundary(), parameter->get_upper_boundary());
			slider->set_digits(parameter->get_digits());
			slider->set_value(parameter->get());

			slider->signal_value_changed().connect( sigc::bind<Gtk::Scale*, IRangeParameter* >( sigc::mem_fun(*this, &ParametersUIBuilder::set_range_parameter), slider, parameter) );

		} else if (IListParameter *parameter = dynamic_cast<IListParameter*>(*it)) {
			// create new combo box for list parameter
			Gtk::Frame *frame = new Gtk::Frame(parameter->get_display_name());
			frame->set_shadow_type(Gtk::SHADOW_NONE);
			Gtk::ComboBoxText *combo_box = new Gtk::ComboBoxText();
			frame->add(*combo_box);
			parameters_box->pack_start(*frame, Gtk::PACK_SHRINK);

			// initialize combo box
			vector<string> items = parameter->get_items();
			vector<string>::iterator it;
			for (it = items.begin(); it != items.end(); ++it) {
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

