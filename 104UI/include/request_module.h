/**
 * Copyright (C) 2016, Vadim Fedorov <coderiks@gmail.com>
 *
 * This program is free software: you can use, modify and/or
 * redistribute it under the terms of the simplified BSD
 * License. You should have received a copy of this license along
 * this program. If not, see
 * <http://www.opensource.org/licenses/bsd-license.html>.
 */

/// Created on: Feb 5, 2014

#ifndef REQUEST_MODULE_H_
#define REQUEST_MODULE_H_

#include "request.h"
#include "i_module.h"

// This class is just a shortcut for the Request<IModule, ...> class.

template<class D>
class RequestModule : public Request<IModule, D>
{
public:
	RequestModule() : Request<IModule, D>()	{ }

	D* get_responce()
	{
		return Request<IModule, D>::get_responce();
	}
};

#endif /* REQUEST_MODULE_H_ */
