/*
 * view.cpp
 *
 *  Created on: Feb 7, 2014
 *      Author: Vadim Fedorov
 */

#include "view.h"

unsigned long View::__next_position__ = 0;

View::View(string title, sigc::slot1<Glib::RefPtr<Gdk::Pixbuf>, int> provider)
: _title(title)
{
	_signal_request_pixbuf.connect( provider );
	_position = __next_position__++;
}


string View::get_title()
{
	return _title;
}


void View::set_title(string value)
{
	_title = value;
}


void View::set_provider(sigc::slot1<Glib::RefPtr<Gdk::Pixbuf>, int> provider)
{
	_signal_request_pixbuf.clear();
	_signal_request_pixbuf.connect( provider );
}


int View::get_position()
{
	return _position;
}


Glib::RefPtr<Gdk::Pixbuf> View::get_pixbuf(unsigned int time)
{
	Glib::RefPtr<Gdk::Pixbuf> pixbuf;
	if (!_signal_request_pixbuf.empty()) {
		pixbuf = _signal_request_pixbuf.emit(time);
	}

	return pixbuf;
}




