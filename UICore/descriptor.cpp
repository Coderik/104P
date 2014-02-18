/*
 * descriptor.cpp
 *
 *  Created on: Feb 6, 2014
 *      Author: Vadim Fedorov
 */

#include "descriptor.h"

Descriptor Descriptor::create()
{
	return Descriptor(new __Descriptor());
}


/**
 * Creates an empty descriptor.
 */
Descriptor Descriptor::empty()
{
	return Descriptor();
}


/**
 * Creates an empty descriptor.
 * Use Descriptor::create() to create a new valid descriptor.
 */
Descriptor::Descriptor()
{
	_descriptor = 0;
}


Descriptor::Descriptor(const Descriptor &other)
{
	_descriptor = other._descriptor;
	if (_descriptor) {
		_descriptor->ref_counter++;
	}
}


Descriptor::~Descriptor()
{
	if (_descriptor) {
		if (_descriptor->ref_counter == 1) {
			delete _descriptor;
		} else {
			_descriptor->ref_counter--;
		}
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
    if (_descriptor) {
		if (_descriptor->ref_counter == 1) {
			delete _descriptor;
		} else {
			_descriptor->ref_counter--;
		}
    }

    // assign new internal descriptor
    _descriptor = other._descriptor;
    if (_descriptor) {
    	_descriptor->ref_counter++;
    }

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

Descriptor::Descriptor(__Descriptor *descriptor)
{
	_descriptor = descriptor;
	_descriptor->ref_counter = 1;
}




