/**
 * Copyright (C) 2016, Vadim Fedorov <coderiks@gmail.com>
 *
 * This program is free software: you can use, modify and/or
 * redistribute it under the terms of the simplified BSD
 * License. You should have received a copy of this license along
 * this program. If not, see
 * <http://www.opensource.org/licenses/bsd-license.html>.
 */

/// Created on: Mar 25, 2013

#ifndef LAYER_H_
#define LAYER_H_

#include <string>
#include <cairomm/context.h>
#include <sigc++/sigc++.h>

class Layer
{
public:
	Layer(std::string id, std::string display_name);
	virtual ~Layer() {}

	std::string get_display_name();
	std::string get_id();
	void set_drawing_size(int width, int height);
	bool set_visibitity(bool is_visible);
	void set_current_time(int time);

	virtual void draw(const Cairo::RefPtr<Cairo::Context>& context);

	typedef sigc::signal<void> type_signal_changed;
	type_signal_changed signal_changed()
	{
		return _signal_changed;
	}
protected:
	Layer() { }
	std::string _display_name;
	std::string _id;
	bool _is_visible;
	int _current_time;
	int _drawing_width, _drawing_height;
	type_signal_changed _signal_changed;

};


#endif /* LAYER_H_ */
