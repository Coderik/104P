/*
 * mask_iterator.h
 *
 *  Created on: Apr 21, 2013
 *      Author: Vadim Fedorov
 */

#ifndef MASK_ITERATOR_H_
#define MASK_ITERATOR_H_

#include <memory>
#include "i_iterable_mask.h"

class MaskIterator
{ 
public:
    typedef std::allocator<Point>::difference_type	difference_type;
    typedef std::allocator<Point>::value_type		value_type;
    typedef std::allocator<Point>::reference		reference;
    typedef std::allocator<Point>::pointer			pointer;
    typedef std::allocator<Point>::const_reference	const_reference;
    typedef std::allocator<Point>::const_pointer	const_pointer;
    typedef std::bidirectional_iterator_tag			iterator_category;

	MaskIterator();
    MaskIterator(IIterableMask *mask, Point current);
    MaskIterator(const MaskIterator& source);
    ~MaskIterator();

    MaskIterator& operator=(const MaskIterator& source);
    bool operator==(const MaskIterator& other) const;
    bool operator!=(const MaskIterator& other) const;
    //bool operator>(const Mask_Iterator&) const;						// Random Access
    //bool operator<(const Mask_Iterator&) const;						// Random Access
    //bool operator<=(const Mask_Iterator&) const;						// Random Access
    //bool operator>=(const Mask_Iterator&) const;						// Random Access

    MaskIterator& operator++();
    MaskIterator operator++(int);
    MaskIterator& operator--();
    MaskIterator operator--(int);
    //Mask_Iterator& operator+=(size_type);								// Random Access
    //Mask_Iterator operator+(size_type) const;							// Random Access
    //friend Mask_Iterator operator+(size_type, const Mask_Iterator&);	// Random Access
    //Mask_Iterator& operator-=(size_type);								// Random Access            
    //Mask_Iterator operator-(size_type) const;							// Random Access
    //difference_type operator-(Mask_Iterator) const;					// Random Access

    const const_reference operator*() const;
    const const_pointer operator->() const;
    //reference operator[](size_type) const;							// Random Access

private:
	IIterableMask *_mask;
	Point _current;
};


 #endif /* MASK_ITERATOR_H_ */