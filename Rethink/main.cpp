/*
 * main.cpp
 *
 *  Created on: 25 марта 2014 г.
 *      Author: Vadim Fedorov
 */

#include <iostream>
#include "image.h"

int main(int argc, char *argv[])
{
	const Image<float> im = Image<float>(5, 5, (float)5.0);
	//im = Image<float>(5, 5, (float)5.0);
	float a = im(2, 2);

	Image<float> im2;
	im2 = im;
	im2(2,2) = 1.5;
	float* data = im2.raw();

	std::cout << "1";
}
