/*
 * main.cpp
 *
 *  Created on: 25 марта 2014 г.
 *      Author: Vadim Fedorov
 */

#include <iostream>
#include "image.h"

void some_method(ConstImage<float> img)
{
	float a = img(2, 2);

	Image<float> another = img;
	another(2,2) = 1.1;
}

int main(int argc, char *argv[])
{
	Image<float> im = Image<float>(5, 5, (float)5.0);
	//im = Image<float>(5, 5, (float)5.0);
	float a = im(2, 2);

	some_method(im);

	float b = im(2, 2);

	//Image<float> im2 = im;
	//const Image<float> im3 = im;
	//im2 = im;
	//im2(2,2) = 1.5;
	//float* data = im2.raw();

	std::cout << "1";
}
