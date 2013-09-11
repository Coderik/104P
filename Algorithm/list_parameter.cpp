/*
 * list_parameter.cpp
 *
 *  Created on: Sep 11, 2013
 *      Author: Vadim Fedorov
 */

#include "headers/list_parameter.h"
#include "headers/steering_patch_weighting.h"

template <class T>
ListParameter<T>::ListParameter()
{
	_display_name = "n/a";
}


template <class T>
ListParameter<T>::ListParameter(string display_name)
{
	_display_name = display_name;
}


template <class T>
ListParameter<T>::ListParameter(string display_name, sigc::slot1<void, T> setter, sigc::slot0<T> getter)
{
	_display_name = display_name;
	_setter = setter;
	_getter = getter;
}


template <class T>
string ListParameter<T>::get_display_name()
{
	return _display_name;
}


template <class T>
vector<string> ListParameter<T>::get_items()
{
	return vector<string>(_item_names);
}


template <class T>
bool ListParameter<T>::set(unsigned int item_id)
{
	if (item_id >= _item_values.size()) {
		return false;
	}

	T value = _item_values[item_id];

	if (!_setter.empty()) {
		_setter(value);
		return true;
	} else {
		return false;
	}
}


template <class T>
int ListParameter<T>::get()
{
	if (!_getter.empty()) {
		T value = _getter();
		for (int i = 0; i < _item_values.size(); i++) {
			if (_item_values[i] == value) {
				return i;
			}
		}
	}

	return 0;
}


template <class T>
void ListParameter<T>::set_display_name(string display_name)
{
	_display_name = display_name;
}


template <class T>
void ListParameter<T>::add_item(string name, T value)
{
	_item_names.push_back(name);
	_item_values.push_back(value);
}


template <class T>
void ListParameter<T>::attach_setter(sigc::slot1<void, T> setter)
{
	_setter = setter;
}


template <class T>
void ListParameter<T>::attach_getter(sigc::slot0<T> getter)
{
	_getter = getter;
}


// Explicit instantiations
template class ListParameter<SteeringPatchWeighting::SteeringModeEnum>;


