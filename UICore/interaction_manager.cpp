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

}




