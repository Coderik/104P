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
	add_events(Gdk::BUTTON_RELEASE_MASK);
	add_events(Gdk::BUTTON1_MOTION_MASK);
	_layer_manager = (Layer_Manager*)0;

	// Create context menu.
	Glib::RefPtr<Gtk::ActionGroup> action_group = Gtk::ActionGroup::create();
	action_group->add(Gtk::Action::create("ContextMenu", "Context Menu"));
	action_group->add(Gtk::Action::create("SaveAction", "Save To PNG"),
						sigc::mem_fun(*this, &Selectable_Image::save_content ));

	_menu_manager = Gtk::UIManager::create();
	_menu_manager->insert_action_group(action_group);

	Glib::ustring ui_info =
		"<ui>"
		"	<popup name='ContextMenu'>"
		"		<menuitem action='SaveAction'/>"
		"  </popup>"
		"</ui>";
	_menu_manager->add_ui_from_string(ui_info);

	// Keep context menu instance.
	_context_menu = dynamic_cast<Gtk::Menu*>(_menu_manager->get_widget("/ContextMenu"));
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

void Selectable_Image::drop_layer_manager()
{
	if (_connection_layer_manager_signal_layer_changed.connected()) {
		_connection_layer_manager_signal_layer_changed.disconnect();
	}

	_layer_manager = static_cast<Layer_Manager* >(0);
}


void Selectable_Image::save_content()
{
	Gtk::FileChooserDialog dialog("Saving content as image...", Gtk::FILE_CHOOSER_ACTION_SAVE);
	Gtk::Window *root_window = static_cast<Gtk::Window* >( this->get_toplevel() );
	dialog.set_transient_for(*root_window);

	// Add response buttons the the dialog:
	dialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
	dialog.add_button(Gtk::Stock::SAVE, Gtk::RESPONSE_OK);

	// Add filters, so that only certain file types can be selected:
	Glib::RefPtr<Gtk::FileFilter> filter_pgm = Gtk::FileFilter::create();
	filter_pgm->set_name("PNG image files (*.png)");
	filter_pgm->add_pattern("*.png");
	dialog.add_filter(filter_pgm);

	// Show the dialog and wait for a user response:
	int result = dialog.run();

	if (result == Gtk::RESPONSE_OK) {
		std::string filename = dialog.get_filename();
		save_content_internal(filename);
	}
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
	if (event->type == GDK_BUTTON_PRESS && event->button == 1) {
		// Left mouse button: select point.
		Gtk::Allocation allocation = get_allocation();
		const int width = allocation.get_width();
		const int height = allocation.get_height();

		double pixbuf_x = (width - _content_width)/2;
		double pixbuf_y = (height - _content_height)/2;

		int x = event->x - pixbuf_x;
		int y = event->y - pixbuf_y;

		if (x > 0 && x < _content_width &&
			y > 0 && y < _content_height) {
			_signal_left_button_pressed.emit(x, y);
		}

		return true;
	} else if (event->type == GDK_BUTTON_PRESS && event->button == 3) {
		// Right mouse button: show context menu;
		if(_context_menu) {
			_context_menu->popup(event->button, event->time);
		}

		return true;
	}

	return false;
}

bool Selectable_Image::on_button_release_event(GdkEventButton *event)
{
	if (event->type == GDK_BUTTON_RELEASE && event->button == 1) {
		// Left mouse button.
		Gtk::Allocation allocation = get_allocation();
		const int width = allocation.get_width();
		const int height = allocation.get_height();

		double pixbuf_x = (width - _content_width)/2;
		double pixbuf_y = (height - _content_height)/2;

		int x = event->x - pixbuf_x;
		int y = event->y - pixbuf_y;

		if (x > 0 && x < _content_width &&
			y > 0 && y < _content_height) {
			_signal_left_button_released.emit(x, y);
		}

		return true;
	}

	return false;
}


bool Selectable_Image::on_motion_notify_event(GdkEventMotion *event)
{
	if (event->type == GDK_MOTION_NOTIFY) {
		Gtk::Allocation allocation = get_allocation();
		const int width = allocation.get_width();
		const int height = allocation.get_height();

		double pixbuf_x = (width - _content_width)/2;
		double pixbuf_y = (height - _content_height)/2;

		int x = event->x - pixbuf_x;
		int y = event->y - pixbuf_y;

		if (x > 0 && x < _content_width &&
			y > 0 && y < _content_height) {
			_signal_left_button_drag.emit(x, y);
		}

		return true;
	}
	return false;
}


/* private */


void Selectable_Image::save_content_internal(const string& filename)
{
	// TODO: add '.png' extension if absent
	// TODO: crop to the actually used area
	Cairo::RefPtr<Cairo::Context > context = this->get_window()->create_cairo_context();
	context->get_target()->write_to_png(filename);
}



