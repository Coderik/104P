/*
 * descriptor.cpp
 *
 *  Created on: Feb 6, 2014
 *      Author: Vadim Fedorov
 */

#include "descriptor.h"

Descriptor Descriptor::create()
{
	return Descriptor();
}


Descriptor::Descriptor(const Descriptor &other)
{
	_descriptor = other._descriptor;
	_descriptor->ref_counter++;
}


Descriptor::~Descriptor()
{
	if (_descriptor->ref_counter == 1) {
		delete _descriptor;
	} else {
		_descriptor->ref_counter--;
	}
}


Descriptor& Descriptor::operator= (const Descriptor &other)
{
    // check for self-assignment by comparing the address of the
    // implicit object and the parameter
    if (this == &other) {
        return *this;
    }

    // finish all deals with the previous internal descriptor
    if (_descriptor->ref_counter == 1) {
		delete _descriptor;
	} else {
		_descriptor->ref_counter--;
	}

    // assign new internal descriptor
    _descriptor = other._descriptor;
    _descriptor->ref_counter++;

    // return the existing object
    return *this;
}


bool Descriptor::operator== (const Descriptor &d) const
{
	return _descriptor == d._descriptor;
}


bool Descriptor::operator!= (const Descriptor &d) const
{
	return !((*this) == d);
}

/* Private */

Descriptor::Descriptor()
{
	_descriptor = new __Descriptor();
	_descriptor->ref_counter = 1;
}




