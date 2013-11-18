/*
 * list_parameter.h
 *
 *  Created on: Sep 11, 2013
 *      Author: Vadim Fedorov
 */

#ifndef LIST_PARAMETER_H_
#define LIST_PARAMETER_H_

#include <sigc++/sigc++.h>
#include "i_list_parameter.h"

using namespace std;

template <class T>
class ListParameter : public IListParameter
{
public:
	ListParameter();
	ListParameter(string display_name);
	ListParameter(string display_name, sigc::slot1<void, T> setter, sigc::slot0<T> getter);
	virtual ~ListParameter() {}

	virtual string get_display_name();
	virtual vector<string> get_items();
	virtual bool set(unsigned int item_id);
	virtual int get();

	void set_display_name(string display_name);
	void add_item(string name, T value);
	void attach_setter(sigc::slot1<void, T> setter);
	void attach_getter(sigc::slot0<T> getter);

private:
	string _display_name;
	vector<string> _item_names;
	vector<T> _item_values;
	sigc::slot1<void, T> _setter;
	sigc::slot0<T> _getter;
};

/* -------------------- Implementation -------------------- */

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


#endif /* LIST_PARAMETER_H_ */
