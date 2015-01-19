/*
 * io_utility.cpp
 *
 *  Created on: Aug 27, 2013
 *      Author: Vadim Fedorov
 */

#include "io_utility.h"

string IOUtility::_prefix = "";

/**
 * Reads image in PGM format from file with provided path\name
 * to the object of 'Image' class
 */
Image<float> IOUtility::read_pgm_image(const string &name)
{
	/* open file */
	// COMPATIBILITY: for win 'rb' file mode instead of just 'r'
	FILE *f = fopen(name.data(),"rb");
	if( f == NULL ) {
		return Image<float>();
	}

	/* read header */
	bool isBinary = false;
	int c, x_size,y_size,depth;

	if ( fgetc(f) != 'P' ) {
		fclose(f);
		return Image<float>();
	}

	if( (c=fgetc(f)) == '2' ) {
		isBinary = false;
	} else if ( c == '5' ) {
		isBinary = true;
	} else {
		fclose(f);
		return Image<float>();
	}

	skip_spaces_and_comments(f);
	fscanf(f,"%d",&x_size);
	skip_spaces_and_comments(f);
	fscanf(f,"%d",&y_size);
	skip_spaces_and_comments(f);
	fscanf(f,"%d",&depth);

	/* get memory */
	Image<float> image(x_size, y_size);

	/* read data */
	skip_spaces_and_comments(f);
	int value;
	for(int y=0;y<y_size;y++) {
		for(int x=0;x<x_size;x++) {
			value = isBinary ? fgetc(f) : get_number(f);
			image(x,y) = value;
		}
	}

	/* close and return */
	fclose(f);

	return image;
}


/**
 * Writes image in PGM format to file with provided path\name
 * from the object of 'Image' class
 */
void IOUtility::write_pgm_image(const string &name, const ImageFx<float> &image)
{
	/* open file */
	FILE *f = fopen(name.data(),"wb");

	/* write header */
	putc('P', f);
	putc('5', f);

	/* write attributes */
	int x_size = image.size_x();
	int y_size = image.size_y();
	fprintf(f, "\n%d %d\n%d\n", x_size, y_size, 255);

	/* write data */
	char value;
	for (int y = 0;y < y_size;y++) {
		for (int x = 0;x < x_size;x++) {
			value = (char)image(x,y);
			putc(value, f);
		}
	}

	/* close file */
	fclose(f);
}


Image<float> IOUtility::read_mono_image(const string &name)
{
	int width, height;

	float *image_data = iio_read_image_float(name.data(), &width, &height);

	if (!image_data) {
		fprintf(stderr, "read_mono_image: cannot read %s\n", name.data());
		return Image<float>();
	}

	Image<float> image(width, height);

	// copy from image_data to Image<float>
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			int index = (y * width + x);
			float value = image_data[index];
			image(x, y) = value;
		}
	}

	free(image_data);

	return image;
}


void IOUtility::write_mono_image(const string &name, const ImageFx<float> &image)
{
	int width = image.size_x();
	int height = image.size_y();

	float *image_data = new float[width * height];

	// copy from Image<float> to image_data
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			int index = (y * width + x);
			image_data[index] = image(x, y);
		}
	}

	iio_save_image_float(const_cast<char* >(name.data()), image_data, width, height);
	delete[] image_data;
}


Image<float> IOUtility::read_rgb_image(const string &name)
{
	int width, height;

	float *image_data = iio_read_image_float_rgb(name.data(), &width, &height);

   if (! image_data) {
      fprintf(stderr, "read_rgb_image: cannot read %s\n", name.data());
      exit(1);
   }

	Image<float> image(width, height, (uint)3);

	// copy from image_data to Image<float>
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			int index = 3 * (y * width + x);
			image(x, y, 0) = image_data[index];
			image(x, y, 1) = image_data[index + 1];
			image(x, y, 2) = image_data[index + 2];
		}
	}

	free(image_data);

	return image;
}


void IOUtility::write_rgb_image(const string &name, const ImageFx<float> &image)
{
	int width = image.size_x();
	int height = image.size_y();

	float *image_data = new float[width * height * 3];

	// copy from Image<float> to image_data
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			int index = 3 * (y * width + x);
			image_data[index + 0] = image(x, y, 0);
			image_data[index + 1] = image(x, y, 1);
			image_data[index + 2] = image(x, y, 2);
		}
	}

	iio_save_image_float_vec(const_cast<char* >(name.data()), image_data, width, height, 3);

   delete[] image_data;
}


void IOUtility::write_float_image(const string &name, const ImageFx<float> &image)
{
	iio_save_image_float_split(const_cast<char*>(name.data()), const_cast<float*>(image.raw()), image.size_x(), image.size_y(), 1);
}


string IOUtility::compose_file_name(const string &name)
{
	return _prefix + name;
}


string IOUtility::compose_file_name(const string &name, int index, const string &extension)
{
	stringstream stream;
	stream << _prefix << name << "_" << setfill('0') << setw(3) << index << "." << extension;
	string file_name = stream.str();
	return file_name;
}


string IOUtility::compose_file_name(const string &name, int index, int index2, const string &extension)
{
	stringstream stream;
	stream << _prefix << name << "_" << index << "_" << setw(3) << setfill('0') << index2 << "." << extension;
	string file_name = stream.str();
	return file_name;
}


void IOUtility::set_prefix(const string &prefix)
{
	_prefix = prefix;
}

/* Private */

void IOUtility::skip_spaces_and_comments(FILE * f)
{
	int c;
	do
	{
		while(isspace(c=fgetc(f))); /* skip spaces */
		if(c=='#') while((c=fgetc(f))!='\n'); /* skip comments */
	}
	while(c == '#');
	ungetc(c,f);
}


/**
 *  Reads a number digit by digit.
 */
int IOUtility::get_number(FILE * f)
	{
	int num, c;

	while(isspace(c=fgetc(f)));
	if(!isdigit(c)) exit(1);
	num = c - '0';
	while( isdigit(c=fgetc(f)) ) num = 10 * num + c - '0';

	return num;
}




