/**
 * Copyright (C) 2016, Vadim Fedorov <coderiks@gmail.com>
 *
 * This program is free software: you can use, modify and/or
 * redistribute it under the terms of the simplified BSD
 * License. You should have received a copy of this license along
 * this program. If not, see
 * <http://www.opensource.org/licenses/bsd-license.html>.
 */

/// Created on: Aug 27, 2013

#ifndef IO_UTILITY_H_
#define IO_UTILITY_H_

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <stdio.h>
#include "tinydir.h"
#include "image.h"
#include "mask.h"
#include "shape.h"

extern "C" {
#include "../iio/iio.h"
}

/**
 * Contains methods for reading and writings images as well as
 * some auxiliary methods. Acts as a proxy for IIO library.
 */
class IOUtility
{
public:
	/// Reading and writing of a grayscale image without IIO.
	static Image<float> read_pgm_image(const std::string &name);
	static void write_pgm_image(const std::string &name, const ImageFx<float> &image);

	/// Reading and writing of a grayscale image using IIO.
	static Image<float> read_mono_image(const std::string &name);
	static void write_mono_image(const std::string &name, const ImageFx<float> &image);

	/// Reading and writing for a colored image using IIO.
	static Image<float> read_rgb_image(const std::string &name);
	static void write_rgb_image(const std::string &name, const ImageFx<float> &image);

	static void write_float_image(const std::string &name, const ImageFx<float> &image);

	static Image<float> read_optical_flow(const std::string &name);
	static void write_optical_flow(const std::string &name, Image<float> flow);

	static std::vector<Image<float> > read_all_flows(const std::string &folder, const std::string &prefix = std::string());

	static Image<float> rgb_to_lab(ImageFx<float> image);
	static Image<float> lab_to_rgb(ImageFx<float> image);
	static Image<float> rgb_to_gray(ImageFx<float> image);
    static Image<float> rgb_to_hsv(ImageFx<float> image);
    static Image<float> hsv_to_rgb(ImageFx<float> image);
    static Image<float> rgb_to_yuv(ImageFx<float> image);
    static Image<float> yuv_to_rgb(ImageFx<float> image);
	static Image<float> to_mono(ImageFx<float> image);
	static Image<float> to_rgb(ImageFx<float> image);

	static Image<float> take_channel(ImageFx<float> image, unsigned int channel_id);
	static Image<float> average_channels(ImageFx<float> image);

	static std::string compose_file_name(const std::string &name);
	static std::string compose_file_name(const std::string &name, int index, const std::string &extension);
	static std::string compose_file_name(const std::string &name, int index, int index2, const std::string &extension);
	static void set_prefix(const std::string &prefix);

private:
    static const float EPS;
	static const float TAG_FLOAT;
	static const std::string TAG_STRING;
	static std::string _prefix;

	static void skip_spaces_and_comments(FILE * f);
	static int get_number(FILE * f);

	static void rgb_to_xyz(const float *rgb, float *xyz);
	static void xyz_to_lab(const float *xyz, float *lab);
	static void lab_to_xyz(const float *lab, float *xyz);
	static void xyz_to_rgb(const float *xyz, float *rgb);
    static void rgb_to_hsv(const float *rgb, float *hsv);
    static void hsv_to_rgb(const float *hsv, float *rgb);
    static void rgb_to_yuv(const float *rgb, float *yuv);
    static void yuv_to_rgb(const float *yuv, float *rgb);
};




#endif /* UTILITY_H_ */
