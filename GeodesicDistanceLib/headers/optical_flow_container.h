/*
 * optical_flow_container.h
 *
 *  Created on: Feb 7, 2013
 *      Author: Vadim Fedorov
 */

#ifndef OPTICAL_FLOW_CONTAINER_H_
#define OPTICAL_FLOW_CONTAINER_H_

class OpticalFlowContainer
{
public:
	OpticalFlowContainer();
	~OpticalFlowContainer();

	float* get_flow_x();
	float* get_flow_y();
	int get_size_x() const;
	int get_size_y() const;
	void set_flow(float *flow_x, float *flow_y, int size_x, int size_y);
	bool contains_data() const;

protected:
	virtual void internal_update() { };

	float *_flow_x, *_flow_y;
	int _size_x, _size_y;
};

#endif /* OPTICAL_FLOW_CONTAINER_H_ */
