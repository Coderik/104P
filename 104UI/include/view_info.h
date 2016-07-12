/**
 * Copyright (C) 2016, Vadim Fedorov <coderiks@gmail.com>
 *
 * This program is free software: you can use, modify and/or
 * redistribute it under the terms of the simplified BSD
 * License. You should have received a copy of this license along
 * this program. If not, see
 * <http://www.opensource.org/licenses/bsd-license.html>.
 */

/// Created on: Feb 6, 2014

#ifndef VIEW_INFO_H_
#define VIEW_INFO_H_

#include <string>
#include "descriptor.h"

using namespace std;

struct ViewInfo
{
public:
	string title;
	Descriptor descriptor;

	ViewInfo(string title, Descriptor descriptor, int position) : title(title), descriptor(descriptor), _position(position) { }

	inline bool operator< (const ViewInfo& other) const
	{
		return _position < other._position;
	}

private:
	int _position;
};


#endif /* VIEW_INFO_H_ */
