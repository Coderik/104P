/*
 * mask_iterator.cpp
 *
 *  Created on: Apr 21, 2013
 *      Author: Vadim Fedorov
 */

#include "headers/mask_iterator.h"

MaskIterator::MaskIterator()
{
	_mask = 0;
	_current = Point(-1, -1, -1); // TODO: add 'empty' static property to Point
}


MaskIterator::MaskIterator(IIterableMask *mask, Point current)
{
	_mask = mask;
	_current = current;
}


MaskIterator::MaskIterator(const MaskIterator& source)
{
	this->_mask = source._mask;
	this->_current = source._current;
}


MaskIterator::~MaskIterator()
{

}


MaskIterator& MaskIterator::operator=(const MaskIterator& source)
{
	if (this != &source) {
		this->_mask = source._mask;
		this->_current = source._current;
	}

	return *this;
}


bool MaskIterator::operator==(const MaskIterator& other) const
{
	return (this->_mask == other._mask) && (this->_current == other._current);
}


bool MaskIterator::operator!=(const MaskIterator& other) const
{
	return (this->_mask != other._mask) || (this->_current != other._current);
}


MaskIterator& MaskIterator::operator++()
{
	if (_mask) {
		_current =  _mask->next(_current);
	}

	return *this;
}


MaskIterator MaskIterator::operator++(int)
{
	MaskIterator aux(*this);

	if (_mask) {
		_current =  _mask->next(_current);
	}

	return aux;
}


MaskIterator& MaskIterator::operator--()
{
	if (_mask) {
		_current =  _mask->prev(_current);
	}

	return *this;
}


MaskIterator MaskIterator::operator--(int)
{
	MaskIterator aux(*this);

	if (_mask) {
		_current =  _mask->prev(_current);
	}

	return aux;
}


const MaskIterator::const_reference MaskIterator::operator*() const
{
	return _current;
}


const MaskIterator::const_pointer MaskIterator::operator->() const
{
	return &_current;
}