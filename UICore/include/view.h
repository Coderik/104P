/*
 * view.h
 *
 *  Created on: Feb 6, 2014
 *      Author: Vadim Fedorov
 */

#ifndef VIEW_H_
#define VIEW_H_

#include <string>
#include <gdkmm/pixbuf.h>
#include <sigc++/sigc++.h>

using namespace std;

class View
{
public:
	View(string title, sigc::slot1<Glib::RefPtr<Gdk::Pixbuf>, int> provider);

	string get_title();
	void set_title(string value);

	void set_provider(sigc::slot1<Glib::RefPtr<Gdk::Pixbuf>, int> provider);

	int get_position();

	Glib::RefPtr<Gdk::Pixbuf> get_pixbuf(unsigned int time = 0);

private:
	static unsigned long __next_position__;	// NOTE: this helps to sort list of views in the order of their creation.
	unsigned long _position;
	string _title;
	sigc::slot1<Glib::RefPtr<Gdk::Pixbuf>, int> _provider;
};



#endif /* VIEW_H_ */
