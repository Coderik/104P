/*
 * utility.h
 *
 *  Created on: Aug 27, 2013
 *      Author: Vadim Fedorov
 */

#ifndef UTILITY_H_
#define UTILITY_H_

#include <string>
#include <sstream>

using namespace std;

class Utility
{
public:
	static string compose_file_name(const string &name, int index, const string &extension);
};




#endif /* UTILITY_H_ */
