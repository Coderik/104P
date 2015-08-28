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
#include "mask.h"
#include "shape.h"

extern "C" {
#include "../iio/iio.h"
}

using namespace std;

/**
 * Contains methods for reading and writings images as well as
 * some auxiliary methods. Acts as a proxy for IIO library.
 */
class IOUtility
{
public:
	/// Reading and writing of a grayscale image without IIO.
	static Image<float> read_pgm_image(const string &name);
	static void write_pgm_image(const string &name, const ImageFx<float> &image);

	/// Reading and writing of a grayscale image using IIO.
	static Image<float> read_mono_image(const string &name);
	static void write_mono_image(const string &name, const ImageFx<float> &image);

	/// Reading and writing for a colored image using IIO.
	static Image<float> read_rgb_image(const string &name);
	static void write_rgb_image(const string &name, const ImageFx<float> &image);

	static void write_float_image(const string &name, const ImageFx<float> &image);

	static Image<float> rgb_to_lab(ImageFx<float> image);
	static Image<float> lab_to_rgb(ImageFx<float> image);
	static Image<float> rgb_to_gray(ImageFx<float> image);

	static string compose_file_name(const string &name);
	static string compose_file_name(const string &name, int index, const string &extension);
	static string compose_file_name(const string &name, int index, int index2, const string &extension);
	static void set_prefix(const string &prefix);

private:
	static string _prefix;

	static void skip_spaces_and_comments(FILE * f);
	static int get_number(FILE * f);

	static void rgb_to_xyz(const float *rgb, float *xyz);
	static void xyz_to_lab(const float *xyz, float *lab);
	static void lab_to_xyz(const float *lab, float *xyz);
	static void xyz_to_rgb(const float *xyz, float *rgb);
};




#endif /* UTILITY_H_ */
