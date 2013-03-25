/*
 * layer.h
 *
 *  Created on: Mar 25, 2013
 *      Author: Vadim Fedorov
 */

#ifndef LAYER_H_
#define LAYER_H_

#include <string>

using namespace std;

class Layer
{
public:
	Layer(string id, string display_name);

	string get_display_name();
	string get_id();
private:
	string _display_name;
	string _id;

};


#endif /* LAYER_H_ */
