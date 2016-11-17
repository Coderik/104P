/**
 * Copyright (C) 2016, Vadim Fedorov <coderiks@gmail.com>
 *
 * This program is free software: you can use, modify and/or
 * redistribute it under the terms of the simplified BSD
 * License. You should have received a copy of this license along
 * this program. If not, see
 * <http://www.opensource.org/licenses/bsd-license.html>.
 */

/// Created on: Jan 23, 2014

#ifndef NUMERICAL_ENTRY_H_
#define NUMERICAL_ENTRY_H_

#include <float.h>
#include <string>
#include <sstream>
#include <iomanip>
#include <gtkmm/entry.h>
#include <gtkmm/spinbutton.h>

class NumericalEntry : public Gtk::Entry
{
public:
	NumericalEntry(unsigned int digits = 0);
	NumericalEntry(Glib::RefPtr<Gtk::Adjustment> &adjustment, unsigned int digits = 0);
	virtual ~NumericalEntry();

	virtual bool on_focus_out_event (GdkEventFocus *event);
	virtual void on_activate();	// NOTE: This is a default handler for the signal, emitted when the user hits the Enter key
	virtual void get_preferred_width_vfunc(int &minimum_width, int &natural_width) const;

	void set_adjustment(const Glib::RefPtr<Gtk::Adjustment>& adjustment);
	Glib::RefPtr<Gtk::Adjustment> get_adjustment();
	void set_digits(unsigned int digits);
	unsigned int get_digits();
	void set_value (double value);
	double get_value();

private:
	static const float EPS;
	static const int MIN_WIDTH;
	Glib::RefPtr<Gtk::Adjustment> _adjustment;
	unsigned int _digits;
	sigc::connection _connection_adjustment_signal_value_changed;
	sigc::connection _connection_adjustment_signal_changed;

	void default_output();
	std::string format_for_value(double value) const;
	void update();
	inline int measure_string_width(Glib::RefPtr<Pango::Layout> &layout, std::string &str) const;

	// slots
	void value_changed();
	void adjustment_changed();

};



#endif /* NUMERICAL_ENTRY_H_ */
