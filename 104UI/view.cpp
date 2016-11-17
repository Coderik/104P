/**
 * Copyright (C) 2016, Vadim Fedorov <coderiks@gmail.com>
 *
 * This program is free software: you can use, modify and/or
 * redistribute it under the terms of the simplified BSD
 * License. You should have received a copy of this license along
 * this program. If not, see
 * <http://www.opensource.org/licenses/bsd-license.html>.
 */

/// Created on: Feb 7, 2014

#include "view.h"

unsigned long View::__next_position__ = 0;

View::View(std::string title, sigc::slot1<Glib::RefPtr<Gdk::Pixbuf>, int> provider)
: _title(title)
{
	_provider = provider;
	_position = __next_position__++;
}


std::string View::get_title()
{
	return _title;
}


void View::set_title(std::string value)
{
	_title = value;
}


void View::set_provider(sigc::slot1<Glib::RefPtr<Gdk::Pixbuf>, int> provider)
{
	_provider = provider;
}


int View::get_position()
{
	return _position;
}


Glib::RefPtr<Gdk::Pixbuf> View::get_pixbuf(unsigned int time)
{
	Glib::RefPtr<Gdk::Pixbuf> pixbuf;
	if (!_provider.empty()) {
		pixbuf = _provider(time);
	}

	return pixbuf;
}




