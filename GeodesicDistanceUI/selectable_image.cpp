/*
 * selectable_image.cpp
 *
 *  Created on: Jan 10, 2013
 *      Author: Vadim Fedorov
 */

#include "selectable_image.h"

Selectable_Image::Selectable_Image()
{
	add_events(Gdk::BUTTON_PRESS_MASK);
	_layer_manager = (Layer_Manager*)0;
}


Selectable_Image::~Selectable_Image()
{

}


void Selectable_Image::set_pixbuf(const Glib::RefPtr<Gdk::Pixbuf>& pixbuf)
{
	_pixbuf = pixbuf;

	// Show at least a quarter of the image.
	if (_pixbuf) {
		_content_width = _pixbuf->get_width();
		_content_height = _pixbuf->get_height();
	    set_size_request(_content_width / 2, _content_height / 2);
	}
}


void Selectable_Image::set_layer_manager(Layer_Manager *layer_manager)
{
	if (_connection_layer_manager_signal_layer_changed.connected()) {
		_connection_layer_manager_signal_layer_changed.disconnect();
	}

	_layer_manager = layer_manager;
	_connection_layer_manager_signal_layer_changed =
			_layer_manager->signal_layer_changed().connect( sigc::mem_fun(*this, &Selectable_Image::queue_draw) );
}


/* protected */

bool Selectable_Image::on_draw(const Cairo::RefPtr<Cairo::Context>& cr)
{
	if (!_pixbuf)
		return false;

	Gtk::Allocation allocation = get_allocation();
	const int width = allocation.get_width();
	const int height = allocation.get_height();

	// Draw the image in the middle of the drawing area, or (if the image is
	// larger than the drawing area) draw the middle part of the image.
	double pixbuf_x = (width - _content_width)/2;
	double pixbuf_y = (height - _content_height)/2;

	Gdk::Cairo::set_source_pixbuf(cr, _pixbuf, pixbuf_x, pixbuf_y);
	cr->paint();

	if (_layer_manager) {
		vector<Layer* > layers = _layer_manager->get_all_layers();
		vector<Layer* >::iterator it;
		for (it = layers.begin(); it != layers.end(); ++it) {
			(*it)->set_drawing_rectangle(pixbuf_x, pixbuf_y, width, height);
			(*it)->draw(cr);
		}
	}
	return true;
}

bool Selectable_Image::on_button_press_event(GdkEventButton *event)
{
	Gtk::Allocation allocation = get_allocation();
	const int width = allocation.get_width();
	const int height = allocation.get_height();

	double pixbuf_x = (width - _content_width)/2;
	double pixbuf_y = (height - _content_height)/2;

	int x = event->x - pixbuf_x;
	int y = event->y - pixbuf_y;

	if (x > 0 && x < _content_width &&
		y > 0 && y < _content_height) {
		_signal_point_selected.emit(x, y);
	}

	return true;
}



