/*
 * console_utility.h
 *
 *  Created on: Sep 25, 2014
 *      Author: Vadim Fedorov
 */

#ifndef CONSOLE_UTILITY_H_
#define CONSOLE_UTILITY_H_

#include <string>
#include <sstream>
#include <vector>

using namespace std;

class ConsoleUtility
{
public:
	static vector<float> parse_float_list(string str);
};



#endif /* CONSOLE_UTILITY_H_ */
