/*
 * console_utility.cpp
 *
 *  Created on: Sep 25, 2014
 *      Author: Vadim Fedorov
 */

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


