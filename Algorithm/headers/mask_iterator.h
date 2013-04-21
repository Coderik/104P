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

class Mask_Iterator
{ 
public:
    typedef std::allocator<Point>::difference_type	difference_type;
    typedef std::allocator<Point>::value_type		value_type;
    typedef std::allocator<Point>::reference		reference;
    typedef std::allocator<Point>::pointer			pointer;
    typedef std::allocator<Point>::const_reference	const_reference;
    typedef std::allocator<Point>::const_pointer	const_pointer;
    typedef std::bidirectional_iterator_tag			iterator_category;

	Mask_Iterator();
    Mask_Iterator(I_Iterable_Mask *mask, Point current);
    Mask_Iterator(const Mask_Iterator& source);
    ~Mask_Iterator();

    Mask_Iterator& operator=(const Mask_Iterator& source);
    bool operator==(const Mask_Iterator& other) const;
    bool operator!=(const Mask_Iterator& other) const;
    //bool operator>(const Mask_Iterator&) const;						// Random Access
    //bool operator<(const Mask_Iterator&) const;						// Random Access
    //bool operator<=(const Mask_Iterator&) const;						// Random Access
    //bool operator>=(const Mask_Iterator&) const;						// Random Access

    Mask_Iterator& operator++();
    Mask_Iterator operator++(int);
    Mask_Iterator& operator--();
    Mask_Iterator operator--(int);
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
	I_Iterable_Mask *_mask;
	Point _current;
};


 #endif /* MASK_ITERATOR_H_ */