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


#endif /* LIST_PARAMETER_H_ */
