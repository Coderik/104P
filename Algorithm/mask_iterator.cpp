/*
 * mask_iterator.cpp
 *
 *  Created on: Apr 21, 2013
 *      Author: Vadim Fedorov
 */

#include "headers/mask_iterator.h"

Mask_Iterator::Mask_Iterator()
{
	_mask = 0;
	_current = Point(-1, -1, -1); // TODO: add 'empty' static property to Point
}


Mask_Iterator::Mask_Iterator(I_Iterable_Mask *mask, Point current)
{
	_mask = mask;
	_current = current;
}


Mask_Iterator::Mask_Iterator(const Mask_Iterator& source)
{
	this->_mask = source._mask;
	this->_current = source._current;
}


Mask_Iterator::~Mask_Iterator()
{

}


Mask_Iterator& Mask_Iterator::operator=(const Mask_Iterator& source)
{
	if (this != &source) {
		this->_mask = source._mask;
		this->_current = source._current;
	}

	return *this;
}


bool Mask_Iterator::operator==(const Mask_Iterator& other) const
{
	return (this->_mask == other._mask) && (this->_current == other._current);
}


bool Mask_Iterator::operator!=(const Mask_Iterator& other) const
{
	return (this->_mask != other._mask) || (this->_current != other._current);
}


Mask_Iterator& Mask_Iterator::operator++()
{
	if (_mask) {
		_current =  _mask->next(_current);
	}

	return *this;
}


Mask_Iterator Mask_Iterator::operator++(int)
{
	Mask_Iterator aux(*this);

	if (_mask) {
		_current =  _mask->next(_current);
	}

	return aux;
}


Mask_Iterator& Mask_Iterator::operator--()
{
	if (_mask) {
		_current =  _mask->prev(_current);
	}

	return *this;
}


Mask_Iterator Mask_Iterator::operator--(int)
{
	Mask_Iterator aux(*this);

	if (_mask) {
		_current =  _mask->prev(_current);
	}

	return aux;
}


const Mask_Iterator::const_reference Mask_Iterator::operator*() const
{
	return _current;
}


const Mask_Iterator::const_pointer Mask_Iterator::operator->() const
{
	return &_current;
}