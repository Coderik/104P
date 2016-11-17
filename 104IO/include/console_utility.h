/**
 * Copyright (C) 2016, Vadim Fedorov <coderiks@gmail.com>
 *
 * This program is free software: you can use, modify and/or
 * redistribute it under the terms of the simplified BSD
 * License. You should have received a copy of this license along
 * this program. If not, see
 * <http://www.opensource.org/licenses/bsd-license.html>.
 */

/// Created on: Sep 25, 2014

#ifndef CONSOLE_UTILITY_H_
#define CONSOLE_UTILITY_H_

#include <string>
#include <sstream>
#include <vector>

class ConsoleUtility
{
public:
	static std::vector<float> parse_float_list(std::string str);
};



#endif /* CONSOLE_UTILITY_H_ */
