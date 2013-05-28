/*
 * layer.h
 *
 *  Created on: Mar 25, 2013
 *      Author: Vadim Fedorov
 */

#ifndef LAYER_H_
#define LAYER_H_

#include <string>
#include <cairomm/context.h>
#include <sigc++/sigc++.h>

using namespace std;

class Layer
{
public:
	Layer(string id, string display_name);
	virtual ~Layer() {}

	string get_display_name();
	string get_id();
	void set_drawing_size(int width, int height);
	bool set_visibitity(bool is_visible);

	virtual void draw(const Cairo::RefPtr<Cairo::Context>& context);

	typedef sigc::signal<void> type_signal_changed;
	type_signal_changed signal_changed()
	{
		return _signal_changed;
	}
protected:
	Layer() { }
	string _display_name;
	string _id;
	bool _is_visible;
	int _drawing_width, _drawing_height;
	type_signal_changed _signal_changed;

};


#endif /* LAYER_H_ */
