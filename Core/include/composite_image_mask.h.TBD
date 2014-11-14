/*
 * composite_image_mask.h
 *
 *  Created on: Sep 25, 2013
 *      Author: Vadim Fedorov
 */

#ifndef COMPOSITE_IMAGE_MASK_H_
#define COMPOSITE_IMAGE_MASK_H_

#include "image_mask.h"

// TODO: should components ne const?
class CompositeImageMask : public ImageMask
{
public:
	enum TypeOfOperation {
		AND,
		OR
	};

	CompositeImageMask();
	CompositeImageMask(TypeOfOperation type_of_operation);
	CompositeImageMask(ImageMask *first,
						bool is_first_inverse,
						ImageMask *second,
						bool is_second_inverse,
						TypeOfOperation type_of_operation);
	virtual ~CompositeImageMask() {}

	virtual bool get_value(int x, int y) const;
	virtual bool get_value(Point p) const;

	bool set_first(ImageMask *component, bool inverse = false);
	bool set_second(ImageMask *component, bool inverse = false);
	void set_type_of_operation(TypeOfOperation value);

private:
	ImageMask *_first_component;
	ImageMask *_second_component;
	bool _first_inverse, _second_inverse;
	TypeOfOperation _operation;

	bool set_component(ImageMask *&new_ref, ImageMask *&existing_ref, bool &new_inverse_ref, ImageMask *component, bool inverse);
};


#endif /* COMPOSITE_IMAGE_MASK_H_ */
