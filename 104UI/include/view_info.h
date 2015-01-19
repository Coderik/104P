/*
 * view_info.h
 *
 *  Created on: Feb 6, 2014
 *      Author: Vadim Fedorov
 */

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
