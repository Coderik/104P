/*
 * selectable_image.cpp
 *
 *  Created on: Jan 10, 2013
 *      Author: Vadim Fedorov
 */

#include "selectable_image.h"

SelectableImage::SelectableImage()
{
	add_events(Gdk::BUTTON_PRESS_MASK);
	add_events(Gdk::BUTTON_RELEASE_MASK);
	//add_events(Gdk::BUTTON1_MOTION_MASK);
	add_events(Gdk::POINTER_MOTION_MASK);
	_layer_manager = (LayerManager*)0;

	// Create context menu.
	Glib::RefPtr<Gtk::ActionGroup> action_group = Gtk::ActionGroup::create();
	action_group->add(Gtk::Action::create("ContextMenu", "Context Menu"));
	action_group->add(Gtk::Action::create("SaveAction", "Save To PNG"),
						sigc::mem_fun(*this, &SelectableImage::save_content ));

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

	_scale = 1;
	_is_common_pan_enabled = false;
	_is_handy_pan_enabled = false;
	_is_dragging = false;
	_is_panning = false;
}


SelectableImage::~SelectableImage()
{

}


void SelectableImage::set_pixbuf(const Glib::RefPtr<Gdk::Pixbuf>& pixbuf)
{
	_content = pixbuf;

	// Show at least a quarter of the image.
	if (_content) {
		_content_width = _content->get_width();
		_content_height = _content->get_height();
		set_size_request(_content_width / 2, _content_height / 2);

		Gtk::Allocation allocation = get_allocation();
		_width = allocation.get_width();
		_height = allocation.get_height();

		// Draw the image in the middle of the drawing area, or (if the image is
		// larger than the drawing area) draw the middle part of the image.
		_scaled_content_width = _content_width * _scale;
		_scaled_content_height = _content_height * _scale;
		_content_x = (_width - _scaled_content_width)/2;
		_content_y = (_height - _scaled_content_height)/2;

		// Align
		_content_x = _scale * (_content_x / (int)_scale);
		_content_y = _scale * (_content_y / (int)_scale);
	}
}


void SelectableImage::set_layer_manager(LayerManager *layer_manager)
{
	if (_connection_layer_manager_signal_layer_changed.connected()) {
		_connection_layer_manager_signal_layer_changed.disconnect();
	}

	_layer_manager = layer_manager;
	_connection_layer_manager_signal_layer_changed =
			_layer_manager->signal_layer_changed().connect( sigc::mem_fun(*this, &SelectableImage::queue_draw) );
}

void SelectableImage::drop_layer_manager()
{
	if (_connection_layer_manager_signal_layer_changed.connected()) {
		_connection_layer_manager_signal_layer_changed.disconnect();
	}

	_layer_manager = static_cast<LayerManager* >(0);
}


bool SelectableImage::set_zoom_scale(short zoom_scale)
{
	if (zoom_scale < MIN_ZOOM_SCALE || zoom_scale > MAX_ZOOM_SCALE) {
		return false;
	}

	_scale = zoom_scale;

	int scaled_content_width = _content_width * _scale;
	int scaled_content_height = _content_height * _scale;

	if (_scaled_content_width != scaled_content_width) {
		if (scaled_content_width < _width || _scaled_content_width < _width) {
			_content_x = (_width - scaled_content_width)/2;
		} else {
			_content_x = _width / 2 - (_width / 2 - _content_x) * _scale;
			_content_x = std::max(_width - _scaled_content_width , std::min(0, _content_x));
		}
		_scaled_content_width = scaled_content_width;
	}

	if (_scaled_content_height != scaled_content_height) {
		if (scaled_content_height < _height || _scaled_content_height < _height) {
			_content_y = (_height - scaled_content_height)/2;
		} else {
			_content_y = _height / 2 - (_height / 2 - _content_y) * _scale;
			_content_y = std::max(_height - _scaled_content_height , std::min(0, _content_y));
		}
		_scaled_content_height = scaled_content_height;
	}

	return true;
}


short SelectableImage::get_zoom_scale()
{
	return (short)_scale;
}


short SelectableImage::get_min_zoom_scale()
{
	return MIN_ZOOM_SCALE;
}


short SelectableImage::get_max_zoom_scale()
{
	return MAX_ZOOM_SCALE;
}


void SelectableImage::set_pan_enabled(bool enabled)
{
	_is_common_pan_enabled = enabled;
}


void SelectableImage::set_handy_pan_enabled(bool enabled)
{
	_is_handy_pan_enabled = enabled;
}


void SelectableImage::save_content()
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

bool SelectableImage::on_draw(const Cairo::RefPtr<Cairo::Context>& cr)
{
	if (!_content)
		return false;

	// NOTE: signal_configure_event might be used to capture size changings
	Gtk::Allocation allocation = get_allocation();
	int width = allocation.get_width();
	int height = allocation.get_height();

	if (_width != width) {
		if (width >= _scaled_content_width || _width >= _scaled_content_width) {
			_content_x = (width - _scaled_content_width)/2;
		} else {
			int dx = (width - _width) / 2;
			_content_x = std::max(width - _scaled_content_width , std::min(0, _content_x + dx));
		}
		_width = width;
	}

	if (_height != height) {
		if (height >= _scaled_content_height || _height >= _scaled_content_height) {
			_content_y = (height - _scaled_content_height)/2;
		} else {
			int dy = (height - _height) / 2;
			_content_y = std::max(height - _scaled_content_height , std::min(0, _content_y + dy));
		}
		_height = height;
	}

	cr->save();

	cr->translate(_content_x, _content_y);

	Glib::RefPtr<Gdk::Pixbuf> pixbuf = _content->scale_simple(_scaled_content_width,
															_scaled_content_height,
															Gdk::INTERP_NEAREST);
	Gdk::Cairo::set_source_pixbuf(cr, pixbuf, 0, 0);
	cr->paint();

	cr->scale(_scale, _scale);

	if (_layer_manager) {
		vector<Layer* > layers = _layer_manager->get_all_layers();
		vector<Layer* >::iterator it;
		for (it = layers.begin(); it != layers.end(); ++it) {
			(*it)->set_drawing_size(_content_width, _content_height);
			(*it)->draw(cr);
		}
	}

	cr->restore();

	return true;
}

bool SelectableImage::on_button_press_event(GdkEventButton *event)
{
	if (event->type == GDK_BUTTON_PRESS && event->button == 1) {
		_is_dragging = true;

		if (is_pan_allowed(event->x, event->y, 30)) {
			// Left mouse button: start panning.
			_is_panning = true;
			_pan_start_x = event->x;
			_pan_start_y = event->y;
			_pan_content_x = _content_x;
			_pan_content_y = _content_y;

			Glib::RefPtr<Gdk::Window> window = this->get_window();
			Glib::RefPtr<Gdk::Cursor> cursor = Gdk::Cursor::create(Gdk::FLEUR);
			window->set_cursor(cursor);
		} else {
			// Left mouse button: select point.
			int x = (event->x - _content_x) / _scale;
			int y = (event->y - _content_y) / _scale;
			if (x > 0 && x < _content_width &&
				y > 0 && y < _content_height) {
				_signal_left_button_pressed.emit(x, y);
			}
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

bool SelectableImage::on_button_release_event(GdkEventButton *event)
{
	if (event->type == GDK_BUTTON_RELEASE && event->button == 1) {
		if (!_is_panning) {
			// Left mouse button.
			int x = (event->x - _content_x) / _scale;
			int y = (event->y - _content_y) / _scale;
			if (x > 0 && x < _content_width &&
				y > 0 && y < _content_height) {
				_signal_left_button_released.emit(x, y);
			}
		}

		_is_dragging = false;
		_is_panning = false;

		Glib::RefPtr<Gdk::Window> window = this->get_window();
		window-> set_cursor();

		return true;
	}

	return false;
}


bool SelectableImage::on_motion_notify_event(GdkEventMotion *event)
{
	if (event->type != GDK_MOTION_NOTIFY) {
		return false;
	}

	if (_is_dragging) {
		if (_is_panning) {
			// Left mouse button: pan.
			int dx = event->x - _pan_start_x;
			int dy = event->y - _pan_start_y;

			if (_scaled_content_width > _width) {
				_content_x = std::max(_width - _scaled_content_width , std::min(0, _pan_content_x + dx));
			}

			if (_scaled_content_height > _height) {
				_content_y = std::max(_height - _scaled_content_height , std::min(0, _pan_content_y + dy));
			}

			this->get_window()->invalidate(false);

		} else {
			// Left mouse button: drag.
			int x = (event->x - _content_x) / _scale;
			int y = (event->y - _content_y) / _scale;
			if (x > 0 && x < _content_width &&
				y > 0 && y < _content_height) {
				_signal_left_button_drag.emit(x, y);
			}
		}

		return true;
	} else {
		// TODO: get rid of blinking on WIN
		/*if (is_pan_allowed(event->x, event->y, 30)) {
			Glib::RefPtr<Gdk::Window> window = this->get_window();
			Glib::RefPtr<Gdk::Cursor> cursor = Gdk::Cursor::create(Gdk::FLEUR);
			window->set_cursor(cursor);
		} else {
			Glib::RefPtr<Gdk::Window> window = this->get_window();
			window-> set_cursor();
		}*/

		return true;
	}

	return false;
}


/* private */


void SelectableImage::save_content_internal(const string& filename)
{
	// TODO: add '.png' extension if absent
	// TODO: crop to the actually used area
	Cairo::RefPtr<Cairo::Context > context = this->get_window()->create_cairo_context();
	context->get_target()->write_to_png(filename);
}


inline bool SelectableImage::is_pan_allowed(int x, int y, int margin)
{
	return _is_common_pan_enabled ||
			(_is_handy_pan_enabled && (x < margin || x > _width - margin || y < margin || y > _height - margin));
}



