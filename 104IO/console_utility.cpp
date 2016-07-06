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

#include "console_utility.h"

vector<float> ConsoleUtility::parse_float_list(string str)
{
	vector<float> float_list;
	vector<string> tokens;
	stringstream stream(str);
    string item;

    while (getline(stream, item, ':')) {
        if (item.size() > 0) {
        	float value = 0.0;
    		stringstream stream(item);
    		stream >> value;
    		if (value > 0) {
    			float_list.push_back(value);
    		}
        }
    }

    return float_list;
}


