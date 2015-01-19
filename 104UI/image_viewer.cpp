/*
 * image_viewer.cpp
 * (Previous name 'selectable_image.cpp'. Renamed on May 28, 2013)
 *
 *  Created on: Jan 10, 2013
 *      Author: Vadim Fedorov
 */

#include "image_viewer.h"

ImageViewer::ImageViewer()
: _is_common_pan_enabled(false),
  _is_handy_pan_enabled(false),
  _is_zoom_by_wheel_enabled(false),
  _show_mouse_coordinates(true),
  _text_color_r(1.0),
  _text_color_g(0.0),
  _text_color_b(0.0),
  _text_color_a(1.0),
  _is_dragging(false),
  _is_panning(false),
  _mouse_x(0),
  _mouse_y(0),
  _scale(1)
{
	add_events(Gdk::BUTTON_PRESS_MASK);
	add_events(Gdk::BUTTON_RELEASE_MASK);
	add_events(Gdk::POINTER_MOTION_MASK);
	add_events(Gdk::SCROLL_MASK);
	_layer_manager = (LayerManager*)0;

	// Create context menu.
	Glib::RefPtr<Gtk::ActionGroup> action_group = Gtk::ActionGroup::create();
	action_group->add(Gtk::Action::create("ContextMenu", "Context Menu"));
	action_group->add(Gtk::Action::create("SaveAction", "Save To PNG"),
						sigc::mem_fun(*this, &ImageViewer::save_content ));
	action_group->add(Gtk::ToggleAction::create("ShowCoordinates", "Show Mouse Coordinates"),
							sigc::mem_fun(*this, &ImageViewer::toggle_show_mouse_coordinates ));

	_menu_manager = Gtk::UIManager::create();
	_menu_manager->insert_action_group(action_group);

	Glib::ustring ui_info =
		"<ui>"
		"	<popup name='ContextMenu'>"
		"		<menuitem action='SaveAction'/>"
		"		<separator/>"
		"		<menuitem action='ShowCoordinates'/>"
		"  </popup>"
		"</ui>";
	_menu_manager->add_ui_from_string(ui_info);

	// Keep context menu instance.
	_context_menu = dynamic_cast<Gtk::Menu*>(_menu_manager->get_widget("/ContextMenu"));
}


ImageViewer::~ImageViewer()
{

}


void ImageViewer::set_pixbuf(const Glib::RefPtr<Gdk::Pixbuf>& pixbuf)
{
	if (!pixbuf) {
		return;
	}

	bool first_data = !_content;
	_content = pixbuf;

	if ( first_data || _content_width != pixbuf->get_width() || _content_height != pixbuf->get_height() ) {
		_content_width = pixbuf->get_width();
		_content_height = pixbuf->get_height();
		set_size_request(_content_width / 2, _content_height / 2);
		_scale = 1;
		center_content();
	}
}


void ImageViewer::set_layer_manager(LayerManager *layer_manager)
{
	if (_connection_layer_manager_signal_layer_changed.connected()) {
		_connection_layer_manager_signal_layer_changed.disconnect();
	}

	_layer_manager = layer_manager;
	_connection_layer_manager_signal_layer_changed =
			_layer_manager->signal_layer_changed().connect( sigc::mem_fun(*this, &ImageViewer::queue_draw) );
}

void ImageViewer::drop_layer_manager()
{
	if (_connection_layer_manager_signal_layer_changed.connected()) {
		_connection_layer_manager_signal_layer_changed.disconnect();
	}

	_layer_manager = static_cast<LayerManager* >(0);
}


void ImageViewer::center_content()
{
	if (_content) {
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


bool ImageViewer::set_zoom_scale(short zoom_scale)
{
	if (zoom_scale < MIN_ZOOM_SCALE || zoom_scale > MAX_ZOOM_SCALE) {
		return false;
	}

	if (_scale == zoom_scale) {
		return true;
	}

	int scaled_content_width = _content_width * zoom_scale;
	int scaled_content_height = _content_height * zoom_scale;

	if (_scaled_content_width != scaled_content_width) {
		if (scaled_content_width < _width || _scaled_content_width < _width) {
			_content_x = (_width - scaled_content_width)/2;
		} else {
			_content_x = _width / 2 - (_width / 2 - _content_x) / _scale * zoom_scale;
			_content_x = std::max(_width - scaled_content_width , std::min(0, _content_x));
		}
		_scaled_content_width = scaled_content_width;
	}

	if (_scaled_content_height != scaled_content_height) {
		if (scaled_content_height < _height || _scaled_content_height < _height) {
			_content_y = (_height - scaled_content_height)/2;
		} else {
			_content_y = _height / 2 - (_height / 2 - _content_y) / _scale * zoom_scale;
			_content_y = std::max(_height - scaled_content_height , std::min(0, _content_y));
		}
		_scaled_content_height = scaled_content_height;
	}

	_scale = zoom_scale;

	this->get_window()->invalidate(false);

	return true;
}


short ImageViewer::get_zoom_scale()
{
	return (short)_scale;
}


short ImageViewer::get_min_zoom_scale()
{
	return MIN_ZOOM_SCALE;
}


short ImageViewer::get_max_zoom_scale()
{
	return MAX_ZOOM_SCALE;
}


void ImageViewer::set_pan_enabled(bool enabled)
{
	_is_common_pan_enabled = enabled;
}


void ImageViewer::set_handy_pan_enabled(bool enabled)
{
	_is_handy_pan_enabled = enabled;
}

void ImageViewer::set_zoom_by_wheel_enabled(bool enabled)
{
	_is_zoom_by_wheel_enabled = enabled;
}


void ImageViewer::set_mouse_coordinates_enabled(bool enabled)
{
	_show_mouse_coordinates = enabled;

	Gtk::Allocation allocation = get_allocation();
	int width = allocation.get_width();
	int height = allocation.get_height();

	// NOTE: we assume that coordinates are drawn in the right bottom corner within the rectangle of size 100x30
	this->get_window()->invalidate_rect(Gdk::Rectangle(width - 100, height - 30, 100, 30), false);
}


void ImageViewer::set_text_color(float r, float g, float b, float a)
{
	_text_color_r = max(0.0f, min(1.0f, r));
	_text_color_g = max(0.0f, min(1.0f, g));
	_text_color_b = max(0.0f, min(1.0f, b));
	_text_color_a = max(0.0f, min(1.0f, a));
}


void ImageViewer::save_content()
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


void ImageViewer::toggle_show_mouse_coordinates()
{
	set_mouse_coordinates_enabled(!_show_mouse_coordinates);
}

/* protected */

bool ImageViewer::on_draw(const Cairo::RefPtr<Cairo::Context>& cr)
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

	if (_show_mouse_coordinates) {
		cr->save();

		int pixel_x = (_mouse_x - _content_x) / _scale;
		int pixel_y = (_mouse_y - _content_y) / _scale;

		cr->set_source_rgba(_text_color_r, _text_color_g, _text_color_b, _text_color_a);
		draw_mouse_coordinates(cr, _width, _height, 10, pixel_x, pixel_y);

		cr->restore();
	}

	return true;
}

bool ImageViewer::on_button_press_event(GdkEventButton *event)
{
	if (event->type == GDK_BUTTON_PRESS && event->button == 1) {
		_is_dragging = true;

		if (is_pan_allowed(event->x, event->y, HANDY_PAN_MARGIN)) {
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
				MouseEvent mouse_event(x, y, event->state);
				_signal_left_button_pressed.emit(mouse_event);
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

bool ImageViewer::on_button_release_event(GdkEventButton *event)
{
	if (event->type == GDK_BUTTON_RELEASE && event->button == 1) {
		if (!_is_panning) {
			// Left mouse button.
			int x = (event->x - _content_x) / _scale;
			int y = (event->y - _content_y) / _scale;
			if (x > 0 && x < _content_width &&
				y > 0 && y < _content_height) {
				MouseEvent mouse_event(x, y, event->state);
				_signal_left_button_released.emit(mouse_event);
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


bool ImageViewer::on_motion_notify_event(GdkEventMotion *event)
{
	if (event->type != GDK_MOTION_NOTIFY) {
		return false;
	}

	if (_show_mouse_coordinates) {
		_mouse_x = event->x;
		_mouse_y = event->y;
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
				MouseEvent mouse_event(x, y, event->state);
				_signal_left_button_drag.emit(mouse_event);
			}
		}

		return true;
	}

	// to update mouse coordinates ask for redrawing, if this has not been done before
	if (_show_mouse_coordinates) {
		Gtk::Allocation allocation = get_allocation();
		int width = allocation.get_width();
		int height = allocation.get_height();

		// NOTE: we assume that coordinates are drawn in the right bottom corner within the rectangle of size 100x30
		this->get_window()->invalidate_rect(Gdk::Rectangle(width - 100, height - 30, 100, 30), false);

		return true;
	}

	return false;
}


bool ImageViewer::on_scroll_event (GdkEventScroll* event)
{
	if (_is_zoom_by_wheel_enabled && _content) {
		if (event->direction == GDK_SCROLL_UP) {
			set_zoom_scale(_scale + 1);
		} else if (event->direction == GDK_SCROLL_DOWN) {
			set_zoom_scale(_scale - 1);
		}

		return true;
	}

	return false;
}


/* private */

void ImageViewer::draw_mouse_coordinates(const Cairo::RefPtr<Cairo::Context>& cr, int width, int height, int margin, int mouse_x, int mouse_y)
{
	stringstream stream;
	stream << mouse_x << " " << mouse_y;
	string coordinates = stream.str();

	Pango::FontDescription font;	// http://developer.gnome.org/pangomm/unstable/classPango_1_1FontDescription.html

	font.set_family("Monospace");
	font.set_weight(Pango::WEIGHT_BOLD);

	Glib::RefPtr<Pango::Layout> layout = create_pango_layout(coordinates);	// http://developer.gnome.org/pangomm/unstable/classPango_1_1Layout.html

	layout->set_font_description(font);

	//get the text dimensions (it updates the variables -- by reference)
	int text_width;
	int text_height;
	layout->get_pixel_size(text_width, text_height);

	// Position the text in the middle
	cr->move_to(width - text_width - margin, height - text_height - margin);

	layout->show_in_cairo_context(cr);
}


void ImageViewer::save_content_internal(const string& filename)
{
	// TODO: add '.png' extension if absent
	// TODO: crop to the actually used area
	Cairo::RefPtr<Cairo::Context > context = this->get_window()->create_cairo_context();
	context->get_target()->write_to_png(filename);
}


inline bool ImageViewer::is_pan_allowed(int x, int y, int margin)
{
	return _is_common_pan_enabled ||
			(_is_handy_pan_enabled && (x < margin || x > _width - margin || y < margin || y > _height - margin));
}



