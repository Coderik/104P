/*
 * range_parameter.h
 *
 *  Created on: Sep 11, 2013
 *      Author: Vadim Fedorov
 */

#ifndef RANGE_PARAMETER_H_
#define RANGE_PARAMETER_H_

#include <sigc++/sigc++.h>
#include "i_range_parameter.h"

class RangeParameter : public IRangeParameter
{
public:
	RangeParameter();
	RangeParameter(string display_name, float lower_boundary, float upper_boundary, float digits);
	RangeParameter(string display_name, float lower_boundary, float upper_boundary, float digits, sigc::slot1<void, float> setter, sigc::slot0<float> getter);
	virtual ~RangeParameter() {}

	virtual string get_display_name();
	virtual float get_lower_boundary();
	virtual float get_upper_boundary();
	virtual int get_digits();
	virtual bool set(float value);
	virtual float get();

	void set_display_name(string display_name);
	void set_lower_boundary(float lower_boundary);
	void set_upper_boundary(float upper_boundary);
	void set_digits(int digits);
	void attach_setter(sigc::slot1<void, float> setter);
	void attach_getter(sigc::slot0<float> getter);

private:
	string _display_name;
	float _lower_boundary;
	float _upper_boundary;
	int _digits;
	sigc::slot1<void, float> _setter;
	sigc::slot0<float> _getter;
};


#endif /* RANGE_PARAMETER_H_ */
