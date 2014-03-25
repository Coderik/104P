/*
 * image_viewer.h
 * (Previous name 'selectable_image.h'. Renamed on May 28, 2013)
 *
 *  Created on: Jan 10, 2013
 *      Author: Vadim Fedorov
 */

#ifndef IMAGE_VIEWER_H_
#define IMAGE_VIEWER_H_

#include <vector>
#include <gdkmm/general.h>
#include <gtkmm/drawingarea.h>
#include <gdkmm/pixbuf.h>
#include <gtkmm/uimanager.h>
#include <gtkmm/menu.h>
#include <gtkmm/filechooserdialog.h>
#include <gtkmm/stock.h>
#include <cairomm/context.h>
#include <sigc++/sigc++.h>
#include "layer_manager.h"
#include "mouse_event.h"

class ImageViewer: public Gtk::DrawingArea
{
public:
	ImageViewer();
	virtual ~ImageViewer();
	void set_pixbuf(const Glib::RefPtr<Gdk::Pixbuf>& pixbuf);
	void set_layer_manager(LayerManager *layer_manager);
	void drop_layer_manager();
	void center_content();

	bool set_zoom_scale(short zoom_scale);
	short get_zoom_scale();
	short get_min_zoom_scale();
	short get_max_zoom_scale();

	void set_pan_enabled(bool enabled);
	void set_handy_pan_enabled(bool enabled);
	void set_zoom_by_wheel_enabled(bool enabled);

	void save_content();

	typedef sigc::signal<void, MouseEvent> type_mouse_signal;
	type_mouse_signal signal_left_button_pressed()
	{
		return _signal_left_button_pressed;
	}

	type_mouse_signal signal_left_button_released()
	{
		return _signal_left_button_released;
	}

	type_mouse_signal signal_left_button_drag()
	{
		return _signal_left_button_drag;
	}

protected:
	//Override default signal handlers:
	virtual bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr);
	virtual bool on_button_press_event(GdkEventButton *event);
	virtual bool on_button_release_event(GdkEventButton *event);
	virtual bool on_motion_notify_event(GdkEventMotion *event);
	virtual bool on_scroll_event (GdkEventScroll* event);


private:
	static const short MAX_ZOOM_SCALE = 6;	// NOTE: this is dictated by memory limitations
	static const short MIN_ZOOM_SCALE = 1;	// TODO: handle zoom out
	static const short HANDY_PAN_MARGIN = 30;

	Glib::RefPtr<Gdk::Pixbuf> _content;
	int _content_x, _content_y;
	int _pan_start_x, _pan_start_y;
	int _pan_content_x, _pan_content_y;
	int _content_width, _content_height;
	int _scaled_content_width, _scaled_content_height;
	int _width, _height;
	bool _is_common_pan_enabled;
	bool _is_handy_pan_enabled;
	bool _is_zoom_by_wheel_enabled;

	bool _is_dragging;
	bool _is_panning;
	type_mouse_signal _signal_left_button_pressed;
	type_mouse_signal _signal_left_button_released;
	type_mouse_signal _signal_left_button_drag;
	LayerManager *_layer_manager;
	sigc::connection _connection_layer_manager_signal_layer_changed;
	Glib::RefPtr<Gtk::UIManager> _menu_manager;
	Gtk::Menu *_context_menu;
	float _scale;

	void save_content_internal(const string& filename);

	inline bool is_pan_allowed(int x, int y, int margin);
};


#endif /* IMAGE_VIEWER_H_ */
