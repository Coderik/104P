/*
 * composite_image_mask.cpp
 *
 *  Created on: Sep 25, 2013
 *      Author: Vadim Fedorov
 */

#include "composite_image_mask.h"

CompositeImageMask::CompositeImageMask()
{
	_points = 0;
	_x_size = 0;
	_y_size = 0;
	_first_component = 0;
	_second_component = 0;
	_first_inverse = false;
	_second_inverse = false;
	_operation = AND;
}


CompositeImageMask::CompositeImageMask(TypeOfOperation type_of_operation)
{
	_points = 0;
	_x_size = 0;
	_y_size = 0;
	_first_component = 0;
	_second_component = 0;
	_first_inverse = false;
	_second_inverse = false;
	_operation = type_of_operation;
}


CompositeImageMask::CompositeImageMask(ImageMask *first,
										bool is_first_inverse,
										ImageMask *second,
										bool is_second_inverse,
										TypeOfOperation type_of_operation)
{
	_points = 0;
	_operation = type_of_operation;
	_first_component = 0;
	_second_component = 0;

	set_component(_first_component, _second_component, _first_inverse, first, is_first_inverse);
	set_component(_second_component, _first_component, _second_inverse, second, is_second_inverse);
}


bool CompositeImageMask::get_value(int x, int y) const
{
	if ( x < 0 || y < 0 || x >= _x_size || y >= _y_size) {
		return false;
	}

	// NOTE: undefined component (zero pointer), is equivalent to empty mask (all values are 'false').
	// NOTE: (variable != is_inverse) is a short way to invert variable's value if is_inverse is true and
	//		remain it the same if is_inverse is false.
	bool first = ( (_first_component) && (_first_component->get_value(x, y)) ) != _first_inverse;
	bool second = ( (_second_component) && (_second_component->get_value(x, y)) ) != _second_inverse;

	if (_operation == AND) {
		return first && second;
	} else if (_operation == OR) {
		return first || second;
	} else {
		return false;
	}
}


bool CompositeImageMask::get_value(Point p) const
{
	return get_value(p.x, p.y);
}


bool CompositeImageMask::set_first(ImageMask *component, bool inverse)
{
	return set_component(_first_component, _second_component, _first_inverse, component, inverse);
}


bool CompositeImageMask::set_second(ImageMask *component, bool inverse)
{
	return set_component(_second_component, _first_component, _second_inverse, component, inverse);
}


void CompositeImageMask::set_type_of_operation(TypeOfOperation value)
{
	_operation = value;
}

/* Private */

bool CompositeImageMask::set_component(ImageMask *&new_ref,
										ImageMask *&existing_ref,
										bool &new_inverse_ref,
										ImageMask *component,
										bool inverse)
{
	if (!component) {
		// drop component
		new_ref = 0;
		if (!existing_ref) {
			_x_size = 0;
			_y_size = 0;
		}
		new_inverse_ref = inverse;
		return true;
	}

	// check if sizes match each other
	if ( existing_ref && (component->get_size() != existing_ref->get_size()) ) {
		return false;
	}

	_x_size = component->get_size_x();
	_y_size = component->get_size_y();

	new_ref = component;
	new_inverse_ref = inverse;

	return true;
}



