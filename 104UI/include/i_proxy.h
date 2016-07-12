/**
 * Copyright (C) 2016, Vadim Fedorov <coderiks@gmail.com>
 *
 * This program is free software: you can use, modify and/or
 * redistribute it under the terms of the simplified BSD
 * License. You should have received a copy of this license along
 * this program. If not, see
 * <http://www.opensource.org/licenses/bsd-license.html>.
 */

/// Created on: Feb 4, 2014

#ifndef I_PROXY_H_
#define I_PROXY_H_

class IProxy
{
public:
	virtual ~IProxy() {}

	virtual void enable(bool should_enable = true) = 0;
	virtual void disable() = 0;
};



#endif /* I_PROXY_H_ */
