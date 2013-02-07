/*
 * selectable_image.h
 *
 *  Created on: Jan 10, 2013
 *      Author: Vadim Fedorov
 */

#ifndef SELECTABLE_IMAGE_H_
#define SELECTABLE_IMAGE_H_

#include <gdkmm/general.h>
#include <gtkmm/drawingarea.h>
#include <gdkmm/pixbuf.h>
#include <cairomm/context.h>
#include <sigc++/sigc++.h>

class Selectable_Image: public Gtk::DrawingArea
{
public:
	Selectable_Image();
	virtual ~Selectable_Image();
	void set_pixbuf(const Glib::RefPtr<Gdk::Pixbuf>& pixbuf);

	typedef sigc::signal<void, int, int> type_signal_point_selected;
	type_signal_point_selected signal_point_selected()
	{
		return _signal_point_selected;
	}

protected:
	//Override default signal handler:
	virtual bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr);
	virtual bool on_button_press_event(GdkEventButton *event);

private:
	Glib::RefPtr<Gdk::Pixbuf> _pixbuf;
	int _content_width, _content_height;
	type_signal_point_selected _signal_point_selected;

};


#endif /* SELECTABLE_IMAGE_H_ */
