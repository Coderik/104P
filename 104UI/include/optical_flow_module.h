/*
 * optical_flow_module.h
 *
 *  Created on: Feb 17, 2014
 *      Author: Vadim Fedorov
 */

#ifndef OPTICAL_FLOW_MODULE_H_
#define OPTICAL_FLOW_MODULE_H_

#include <gtkmm/menuitem.h>
#include <gtkmm/separatormenuitem.h>

#include "i_module.h"
#include "i_optical_flow_provider.h"
#include "i_optical_flow_aware.h"
#include "optical_flow.h"
#include "optical_flow_io.h"
#include "optical_flow_io_legacy.h"
#include "optical_flow_data.h"
#include "background_worker.h"
#include "zach_TVL1_optical_flow.h"

class OpticalFlowModule : public IModule, public IOpticalFlowProvider
{
public:
	virtual ~OpticalFlowModule() {}

	virtual void initialize(IModulable *modulable);

	virtual vector<OpticalFlowContainer*> request_forward_optical_flow();
	virtual vector<OpticalFlowContainer*> request_backward_optical_flow();
	virtual bool request_has_optical_flow_data();

private:
	IModulable *_modulable;
	Gtk::Menu *_menu;
	Gtk::MenuItem *_restore_menu_item;
	Gtk::MenuItem *_calculate_menu_item;
	Gtk::MenuItem *_proceed_menu_item;
	std::vector<OpticalFlowContainer*> _forward_optical_flow_list;
	std::vector<OpticalFlowContainer*> _backward_optical_flow_list;
	IBackgroundWorker *_background_worker;
	std::vector<int> _task_list;
	int _frames_amount;
	int _progress_counter, _progress_total;
	bool _has_optical_flow_data;
	bool _optical_flow_legacy_format;
	Descriptor _forward_optical_flow_vector_view;
	Descriptor _forward_optical_flow_magnitude_view;
	Descriptor _backward_optical_flow_vector_view;
	Descriptor _backward_optical_flow_magnitude_view;
	Descriptor _background_work_info;

	// Slots
	void sequence_changed();
	void restore_optical_flow();
	void begin_full_optical_flow_calculation();
	void begin_missing_optical_flow_calculation();
	Glib::RefPtr<Gdk::Pixbuf> provide_forward_optical_flow_vector_view(unsigned int time);
	Glib::RefPtr<Gdk::Pixbuf> provide_forward_optical_flow_magnitude_view(unsigned int time);
	Glib::RefPtr<Gdk::Pixbuf> provide_backward_optical_flow_vector_view(unsigned int time);
	Glib::RefPtr<Gdk::Pixbuf> provide_backward_optical_flow_magnitude_view(unsigned int time);

	template <typename T>
	void reset_vector_of_pointers(std::vector<T*> &v, int size);
	void begin_optical_flow_calculation_internal(std::vector<int> task_list);
	void calculate_optical_flow(IBackgroundInsider *insider, std::vector<int> task_list);
	void take_optical_flow_frame(IData *data);
	void end_calculate_optical_flow();
	void store_optical_flow(OpticalFlowContainer &flow, int index);
	void cancel_calculate_optical_flow();
	void fill_task_list(std::vector<OpticalFlowContainer*> &forward_flow, std::vector<OpticalFlowContainer*> &backward_flow, std::vector<int> &task_list);
	void notify_changes();
	void add_optical_flow_views();
	void remove_optical_flow_views();

};




#endif /* OPTICAL_FLOW_MODULE_H_ */
