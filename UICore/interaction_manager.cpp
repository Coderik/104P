/*
 * interaction_manager.cpp
 *
 *  Created on: Oct 1, 2013
 *      Author: Vadim Fedorov
 */

#include "interaction_manager.h"

InteractionManager::InteractionManager()
{
	_ui = 0;
	_active = 0;
}


Gtk::Box* InteractionManager::get_ui()
{
	return _ui;
}


Interaction* InteractionManager::get_active()
{
	return _active;
}


void InteractionManager::add_interation(Interaction *interaction)
{
	_interactions.push_back(interaction);

	if (_interactions.size() == 1) {
		_active = interaction;
	}

	// update ui
	Gtk::Box* obsolete_ui = _ui;
	_ui = build_ui();
	_signal_ui_updated.emit();

	// destroy obsolete ui
	destroy_ui(obsolete_ui);
}


int InteractionManager::get_interactions_amount()
{
	return _interactions.size();
}

/* Private */

Gtk::Box* InteractionManager::build_ui()
{
	if (_interactions.size() <= 1) {
		return 0;
	}

	Gtk::Box* ui = new Gtk::VBox();
	Gtk::Frame* toolbar_frame = new Gtk::Frame();
	ui->pack_start(*toolbar_frame, Gtk::PACK_EXPAND_WIDGET);

	Gtk::Toolbar *toolbar = new Gtk::Toolbar();
	toolbar->set_property("orientation", Gtk::ORIENTATION_VERTICAL);
	toolbar->set_toolbar_style(Gtk::TOOLBAR_ICONS);
	toolbar_frame->add(*toolbar);

	Gtk::RadioButtonGroup group = Gtk::RadioButtonGroup();

	vector<Interaction* >::iterator it;
	for (it = _interactions.begin(); it != _interactions.end(); ++it) {
		// TODO: use user-provided icons
		Gtk::RadioToolButton *button = new Gtk::RadioToolButton(group, Gtk::Stock::ABOUT);
		group = button->get_group();	// NOTE: this magic is required. Without it all buttons will be in separate groups.
		button->set_tooltip_text((*it)->get_display_name());

		if ((*it) == _active) {
			button->activate();
		}

		button->signal_clicked().connect(
					sigc::bind<Interaction* >(
							sigc::mem_fun(*this, &InteractionManager::set_active),
							*it ) );

		toolbar->append(*button);
	}

	return ui;
}


void InteractionManager::destroy_ui(Gtk::Box* ui)
{
	if (ui == 0) {
		return;
	}

	vector<Gtk::Widget* > children = ui->get_children();
	vector<Gtk::Widget* >::iterator it;
	for(it = children.begin(); it != children.end(); ++it) {
		ui->remove(**it);
		delete *it;
	}

	delete ui;
}


void InteractionManager::set_active(Interaction *interaction)
{
	_active = interaction;
}




