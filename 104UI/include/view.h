/**
 * Copyright (C) 2016, Vadim Fedorov <coderiks@gmail.com>
 *
 * This program is free software: you can use, modify and/or
 * redistribute it under the terms of the simplified BSD
 * License. You should have received a copy of this license along
 * this program. If not, see
 * <http://www.opensource.org/licenses/bsd-license.html>.
 */

/// Created on: Feb 6, 2014

#ifndef VIEW_H_
#define VIEW_H_

#include <string>
#include <gdkmm/pixbuf.h>
#include <sigc++/sigc++.h>

class View
{
public:
	View(std::string title, sigc::slot1<Glib::RefPtr<Gdk::Pixbuf>, int> provider);

	std::string get_title();
	void set_title(std::string value);

	void set_provider(sigc::slot1<Glib::RefPtr<Gdk::Pixbuf>, int> provider);

	int get_position();

	Glib::RefPtr<Gdk::Pixbuf> get_pixbuf(unsigned int time = 0);

private:
	static unsigned long __next_position__;	// NOTE: this helps to sort list of views in the order of their creation.
	unsigned long _position;
	std::string _title;
	sigc::slot1<Glib::RefPtr<Gdk::Pixbuf>, int> _provider;
};



#endif /* VIEW_H_ */
