/*
 * request_module.h
 *
 *  Created on: Feb 5, 2014
 *      Author: Vadim Fedorov
 */

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
