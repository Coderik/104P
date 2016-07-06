/**
 * Copyright (C) 2016, Vadim Fedorov <coderiks@gmail.com>
 *
 * This program is free software: you can use, modify and/or
 * redistribute it under the terms of the simplified BSD
 * License. You should have received a copy of this license along
 * this program. If not, see
 * <http://www.opensource.org/licenses/bsd-license.html>.
 */

/// Created on: Feb 4, 2014

#include "ui_container.h"

void UI_Container::setup_ui(Gtk::Window* window, string application_id)
{
	_application_id = application_id;

	// adjust main window
	window->set_title("Image & video processing testing environment");
	window->set_default_size(920, 800);

	// set up menu
	_menu_manager = Gtk::UIManager::create();

	Glib::RefPtr<Gtk::ActionGroup> action_group = Gtk::ActionGroup::create();
	action_group->add(Gtk::Action::create("FileMenu", "File"));
	open_image_action = Gtk::Action::create("OpenImage", "Open Image File...");
	action_group->add(open_image_action);
	open_sequence_action = Gtk::Action::create("OpenSequence", "Open Sequence Folder...");
	action_group->add(open_sequence_action);
	open_recent_action = Gtk::RecentAction::create("OpenRecent", "Open Recent");
	Glib::RefPtr<Gtk::RecentFilter> filter = Gtk::RecentFilter::create();
	filter->add_application(_application_id);
	open_recent_action->add_filter(filter);
	action_group->add(open_recent_action);
	quit_action = Gtk::Action::create("Quit", Gtk::Stock::QUIT);
	action_group->add(quit_action);
	_menu_manager->insert_action_group(action_group);

	Glib::RefPtr<Gtk::ActionGroup> view_action_group = Gtk::ActionGroup::create();
	view_action_group->add(Gtk::Action::create("ViewMenu", "View"));
	_menu_manager->insert_action_group(view_action_group);

	fitting_action_group = Gtk::ActionGroup::create();
	fitting_action_group->add(Gtk::Action::create("FittingMenu", "Fitting"));
	_menu_manager->insert_action_group(fitting_action_group);

	layer_action_group = Gtk::ActionGroup::create();
	layer_action_group->add(Gtk::Action::create("LayerMenu", "Layers"));
	layers_visibility_toggle_action = Gtk::ToggleAction::create("LayersVisibility", "Show all layers");
	layer_action_group->add(layers_visibility_toggle_action);
	_menu_manager->insert_action_group(layer_action_group);
	layer_action_group->set_sensitive(false);

	Glib::ustring ui_info =
		"<ui>"
		"  <menubar name='MenuBar'>"
		"    <menu action='FileMenu'>"
		"      <menuitem action='OpenImage'/>"
		"      <menuitem action='OpenSequence'/>"
		"      <menuitem action='OpenRecent'/>"
		"      <separator/>"
		"      <menuitem action='Quit'/>"
		"    </menu>"
		"    <menu action='ViewMenu'>"
		"    </menu>"
		"    <menu action='LayerMenu'>"
		"      <menuitem action='LayersVisibility'/>"
		"    </menu>"
		"    <menu action='FittingMenu'>"
		"    </menu>"
		"  </menubar>"
		"</ui>";
	_menu_manager->add_ui_from_string(ui_info);

	// create layout
	Gtk::Box *window_layout = new Gtk::VBox();		// containes menubar, infobar, main working area and statusbar
	window->add(*window_layout);

	_menu_bar = static_cast<Gtk::MenuBar*>(_menu_manager->get_widget("/MenuBar"));
	if (_menu_bar) {
		window_layout->pack_start(*_menu_bar, Gtk::PACK_SHRINK);
	}

	_view_menu_item = dynamic_cast<Gtk::MenuItem*>(_menu_manager->get_widget("/MenuBar/ViewMenu"));

	background_work_infobar = new Gtk::InfoBar();
	background_work_infobar->set_message_type(Gtk::MESSAGE_INFO);
	window_layout->pack_start(*background_work_infobar, Gtk::PACK_SHRINK);

	status_bar = new Gtk::Statusbar();
	window_layout->pack_end(*status_bar,  Gtk::PACK_SHRINK);

	Gtk::Box *working_area_layout = new Gtk::HBox();	// containes central, right and left 'sides'
	working_area_layout->set_margin_top(10);
	working_area_layout->set_margin_left(10);
	working_area_layout->set_margin_right(10);
	window_layout->pack_start(*working_area_layout, Gtk::PACK_EXPAND_WIDGET);

	left_side_layout = new Gtk::VBox();		// toolbox placeholder
	working_area_layout->pack_start(*left_side_layout, Gtk::PACK_SHRINK);

	Gtk::Box *central_layout = new Gtk::VBox();		// containes image/sequence view and time scale
	central_layout->set_margin_left(10);
	central_layout->set_margin_right(10);
	working_area_layout->pack_start(*central_layout, Gtk::PACK_EXPAND_WIDGET);

	Gtk::Frame *image_view_frame = new Gtk::Frame("Image/sequence view");
	image_view_frame->set_label_align(Gtk::ALIGN_CENTER, Gtk::ALIGN_START);
	image_view_frame->set_shadow_type(Gtk::SHADOW_ETCHED_OUT);
	central_layout->pack_start(*image_view_frame, Gtk::PACK_EXPAND_WIDGET);

	time_slider = new Gtk::Scale(Gtk::ORIENTATION_HORIZONTAL);
	central_layout->pack_start(*time_slider, Gtk::PACK_SHRINK);

	right_side_layout = new Gtk::VBox();		// rig placeholder
	working_area_layout->pack_start(*right_side_layout, Gtk::PACK_SHRINK);

	// fill layout with controls
	background_work_infobar->add_button(Gtk::Stock::CANCEL, 0);
	Gtk::Container* infobar_container =
				dynamic_cast<Gtk::Container*>(background_work_infobar->get_content_area());
	if (infobar_container) {
		Gtk::HBox *infobar_box = new Gtk::HBox();
		infobar_container->add(*infobar_box);
		Gtk::Label *infobar_caption = new Gtk::Label("Background calculations. ");
		infobar_box->pack_start(*infobar_caption, Gtk::PACK_SHRINK);
		background_work_infobar_message = new Gtk::Label();
		infobar_box->pack_start(*background_work_infobar_message, Gtk::PACK_EXPAND_PADDING);
	}

	image_control = new ImageViewer();
	image_view_frame->add(*image_control);

	// show all
	window->show_all_children();
}


void UI_Container::add_recent_file(string filename)
{
	add_recent_document_internal(filename, "");
}


void UI_Container::add_recent_folder(string path)
{
	add_recent_document_internal(path, "inode/directory");
}


void UI_Container::refresh_placeholders()
{
	right_side_layout->show_all_children(true);
	left_side_layout->show_all_children(true);
}


void UI_Container::clear_placeholders()
{
	vector<Gtk::Widget* > children = right_side_layout->get_children();
	vector<Gtk::Widget* >::iterator it;
	for(it = children.begin(); it != children.end(); ++it) {
		right_side_layout->remove(**it);
	}

	children = left_side_layout->get_children();
	for(it = children.begin(); it != children.end(); ++it) {
		left_side_layout->remove(**it);
	}
}


void UI_Container::assign_menu(Gtk::Menu *menu, string title)
{
	Gtk::MenuItem *main_menu_item = Gtk::manage(new Gtk::MenuItem(title));
	main_menu_item->set_submenu(*menu);

	_menu_bar->insert(*main_menu_item, 1);
	_menu_bar->show_all_children(true);
}


void UI_Container::update_veiw_menu(const vector<ViewInfo> &view_infos, Descriptor active)
{
	Gtk::Menu *menu = Gtk::manage(new Gtk::Menu());

	Gtk::RadioAction::Group group = Gtk::RadioAction::Group();
	vector<ViewInfo>::const_iterator it;
	for (it = view_infos.begin(); it != view_infos.end(); ++it) {
		Gtk::RadioMenuItem *item = Gtk::manage(new Gtk::RadioMenuItem(group, it->title));
		if (it->descriptor == active) {
			item->activate();
		}
		item->signal_toggled().connect( sigc::bind<Descriptor, Gtk::RadioMenuItem* >(sigc::mem_fun(*this, &UI_Container::view_changed_internal), it->descriptor, item) );
		menu->append(*item);
	}

	_view_menu_item->set_submenu(*menu);
	_view_menu_item->show_all();
}


void UI_Container::set_fittings(std::vector<Fitting* > fittings)
{
	// TODO: handle multiple call (do nothing or reinitialize)

	if (fittings.size() == 0) {
		return;
	}

	Glib::ustring ui_info =
		"<ui>"
		"  <menubar name='MenuBar'>"
		"    <menu action='FittingMenu'>";


	Gtk::RadioAction::Group group = Gtk::RadioAction::Group();
	Glib::RefPtr<Gtk::RadioAction> action;
	std::string name;
	for (unsigned int i = 0; i < fittings.size(); i++) {
		// int to string
		std::stringstream s;
		s << i;
		s >> name;

		_fitting_map[name] = fittings[i];
		action = Gtk::RadioAction::create(group, name, fittings[i]->display_name);
		ui_info += Glib::ustring::compose ("      <menuitem action='%1'/>", name);
		fitting_action_group->add(action);
	}

	ui_info +=
		"    </menu>"
		"  </menubar>"
		"</ui>";

	_fitting_submenu_merge_id = _menu_manager->add_ui_from_string (ui_info);
	//_menu_manager->insert_action_group(fitting_action_group);

	action->signal_changed().connect( sigc::mem_fun(*this, &UI_Container::set_fitting_internal) );

	_current_fitting = fittings[0];
	_signal_fitting_changed.emit();
}


Fitting* UI_Container::get_fitting()
{
	return _current_fitting;
}

/* Private */

void UI_Container::add_recent_document_internal(string path, string mime_type)
{
	Glib::RefPtr<Gtk::RecentManager> recent_manager = Gtk::RecentManager::get_default();
	Gtk::RecentManager::Data metadata;

	string uri = Glib::filename_to_uri(path);

	metadata.display_name = Glib::filename_display_basename(path);
	metadata.mime_type = mime_type;		// NOTE: it seems that on Windows this mime type is ignored
	metadata.app_name = _application_id;
	metadata.is_private = true;

	recent_manager->add_item(uri, metadata);
}


void UI_Container::view_changed_internal(const Descriptor& current, Gtk::RadioMenuItem* item)
{
	// NOTE: this method is called twice: first for the deactivated menu item and second for the activated one.
	if (item->get_active()) {
		_signal_active_view_changed.emit(current);
	}
}

void UI_Container::set_fitting_internal(const Glib::RefPtr<Gtk::RadioAction>& current)
{
	Glib::ustring name = current->get_name();

	_current_fitting = _fitting_map[name];

	_signal_fitting_changed.emit();
}


