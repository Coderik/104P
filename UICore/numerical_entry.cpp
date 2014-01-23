/*
 * numerical_entry.cpp
 *
 *  Created on: Jan 23, 2014
 *      Author: Vadim Fedorov
 */

#include "numerical_entry.h"

const int NumericalEntry::MIN_WIDTH;

NumericalEntry::NumericalEntry(unsigned int digits)
: Gtk::Entry()
{
	_digits = digits;
	_adjustment = Gtk::Adjustment::create(0,0,0,0,0,0);
}


NumericalEntry::NumericalEntry(Glib::RefPtr<Gtk::Adjustment> &adjustment, unsigned int digits)
: Gtk::Entry()
{
	_digits = digits;
	_adjustment = adjustment;
}


NumericalEntry::~NumericalEntry()
{

}


bool NumericalEntry::on_focus_out_event (GdkEventFocus *event)
{
	if (get_editable()) {
		update();
	}

	return Gtk::Entry::on_focus_out_event(event);
}


void NumericalEntry::on_activate()
{
	if (get_editable()) {
		update();
	}

	Gtk::Entry::on_activate();
}


void NumericalEntry::get_preferred_width_vfunc(int &minimum_width, int &natural_width) const
{
	Glib::RefPtr<Gtk::StyleContext> style_context = get_style_context();
	Glib::RefPtr<const Pango::Layout> layout = get_layout();
	Glib::RefPtr<Pango::Layout> layout_copy = Pango::Layout::create(layout->get_context());

	// measure width of a string containing the upper value
	string upper_str = format_for_value(_adjustment->get_upper());
	int upper_width = measure_string_width(layout_copy, upper_str);

	int width = max(upper_width, MIN_WIDTH);

	// measure width of a string containing the lower value
	string lower_str = format_for_value(_adjustment->get_lower());
	int lower_width = measure_string_width(layout_copy, lower_str);

	width = max(width, lower_width);

	// add borders
	Gtk::Border border = style_context->get_border();
	width += border.get_left() + border.get_right();

	// add paddings
	Gtk::Border padding = style_context->get_padding();
	width += padding.get_left() + padding.get_right();

	minimum_width = width;
	natural_width = width;
}


void NumericalEntry::set_adjustment(const Glib::RefPtr<Gtk::Adjustment>& adjustment)
{
	if (_adjustment != adjustment) {
		if (_adjustment) {
			if (_connection_adjustment_signal_value_changed.connected()) {
				_connection_adjustment_signal_value_changed.disconnect();
			}
			if (_connection_adjustment_signal_changed.connected()) {
				_connection_adjustment_signal_changed.disconnect();
			}
		}

		_adjustment = adjustment;	// TODO: or store locally also as a RefPtr?

		if (_adjustment) {
			_connection_adjustment_signal_value_changed =
					_adjustment->signal_value_changed().connect( sigc::mem_fun(*this, &NumericalEntry::value_changed) );
			_connection_adjustment_signal_changed =
					_adjustment->signal_changed().connect( sigc::mem_fun(*this, &NumericalEntry::adjustment_changed) );
		}

		g_object_notify(G_OBJECT (this), "adjustment");	// TODO: do we need this?
	}
}


Glib::RefPtr<Gtk::Adjustment> NumericalEntry::get_adjustment()
{
	return _adjustment;
}


void NumericalEntry::set_digits(unsigned int digits)
{
	if (_digits != digits) {
		_digits = digits;
		value_changed();

		g_object_notify (G_OBJECT (this), "digits");	// TODO: do we need this?

		// since lower/upper may have changed
		queue_resize();
	}
}


unsigned int NumericalEntry::get_digits()
{
	return _digits;
}


void NumericalEntry::set_value (double value)
{
	if (abs(value - _adjustment->get_value()) > EPS) {
		_adjustment->set_value(value);
	} else {
		//gint return_val = FALSE;
		//g_signal_emit (spin_button, spinbutton_signals[OUTPUT], 0, &return_val);
		//if (return_val == FALSE)
		default_output();
    }
}


double NumericalEntry::get_value()
{
	return _adjustment->get_value();
}

/* Private */

void NumericalEntry::default_output()
{
	double value = _adjustment->get_value();
	string str_value = format_for_value(value);

	if (str_value.compare(get_text()) != 0) {
		set_text(str_value);
	}
}


string NumericalEntry::format_for_value(double value) const
{
	stringstream s;

	s << fixed << setprecision(_digits) << value;

	return s.str();
}


void NumericalEntry::update()
{
	// get value as double
	string str = get_text();
	stringstream s(get_text());
	double value;
	bool is_valid = s >> value;

	if (is_valid) {
		// restrict value between lower and upper limits
		value = max(_adjustment->get_lower(), min(_adjustment->get_upper(), value));
		set_value(value);
	} else {
		// revert to the previous text
		default_output();
	}

	queue_draw();
}


inline int NumericalEntry::measure_string_width(Glib::RefPtr<Pango::Layout> &layout, string &str) const
{
	int width, heights;
	layout->set_text(str);
	layout->get_pixel_size(width, heights);
	return width;
}


void NumericalEntry::value_changed()
{
	// bool return_val = false;
	//g_signal_emit (spin_button, spinbutton_signals[OUTPUT], 0, &return_val);
	// if (!return_val)

	default_output();

	//g_signal_emit (spin_button, spinbutton_signals[VALUE_CHANGED], 0);

	queue_draw();

	g_object_notify(G_OBJECT(this), "value");
}


void NumericalEntry::adjustment_changed()
{
	queue_resize();	// TODO: override size()
}




