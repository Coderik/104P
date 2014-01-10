/*
 * utility.cpp
 *
 *  Created on: Aug 27, 2013
 *      Author: Vadim Fedorov
 */

#include "utility.h"

string Utility::compose_file_name(const string &name, int index, const string &extension)
{
	stringstream stream;
	stream << name << "_" << index << "." << extension;
	string file_name = stream.str();
	return file_name;
}




