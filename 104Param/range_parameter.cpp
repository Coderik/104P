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

#include "range_parameter.h"

RangeParameter::RangeParameter()
{
	_display_name = "n/a";
	_lower_boundary = 0.0;
	_upper_boundary = 1.0;
	_digits = 0.1;
}


RangeParameter::RangeParameter(string display_name, float lower_boundary, float upper_boundary, float digits)
{
	_display_name = display_name;
	_lower_boundary = lower_boundary;
	_upper_boundary = upper_boundary;
	_digits = digits;
}


RangeParameter::RangeParameter(string display_name, float lower_boundary, float upper_boundary, float digits, sigc::slot1<void, float> setter, sigc::slot0<float> getter)
{
	_display_name = display_name;
	_lower_boundary = lower_boundary;
	_upper_boundary = upper_boundary;
	_digits = digits;
	_setter = setter;
	_getter = getter;
}


string RangeParameter::get_display_name()
{
	return _display_name;
}


float RangeParameter::get_lower_boundary()
{
	return _lower_boundary;
}


float RangeParameter::get_upper_boundary()
{
	return _upper_boundary;
}


int RangeParameter::get_digits()
{
	return _digits;
}


bool RangeParameter::set(float value)
{
	if (value < _lower_boundary || value > _upper_boundary) {
		return false;
	}

	if (!_setter.empty()) {
		_setter(value);
		return true;
	} else {
		return false;
	}
}


float RangeParameter::get()
{
	if (!_getter.empty()) {
		return _getter();
	} else {
		return _lower_boundary;
	}
}


void RangeParameter::set_display_name(string display_name)
{
	_display_name = display_name;
}


void RangeParameter::set_lower_boundary(float lower_boundary)
{
	_lower_boundary = lower_boundary;
}


void RangeParameter::set_upper_boundary(float upper_boundary)
{
	_upper_boundary = upper_boundary;
}


void RangeParameter::set_digits(int digits)
{
	_digits = digits;
}


void RangeParameter::attach_setter(sigc::slot1<void, float> setter)
{
	_setter = setter;
}


void RangeParameter::attach_getter(sigc::slot0<float> getter)
{
	_getter = getter;
}
