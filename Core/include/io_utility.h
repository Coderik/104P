/*
 * io_utility.h
 *
 *  Created on: Aug 27, 2013
 *      Author: Vadim Fedorov
 */

#ifndef IO_UTILITY_H_
#define IO_UTILITY_H_

#include <string>
#include <sstream>
#include <iomanip>
#include <stdio.h>

#include "image.h"
#include "image_mask.h"
#include "shape.h"

using namespace std;

class IOUtility
{
public:
	static Image<float>* read_pgm_image(const string &name);
	static void write_pgm_image(const string &name, Image<float> *image);

	static string compose_file_name(const string &name, int index, const string &extension);
	static string compose_file_name(const string &name, int index, int index2, const string &extension);

private:
	static void skip_spaces_and_comments(FILE * f);
	static int get_number(FILE * f);
};




#endif /* UTILITY_H_ */
