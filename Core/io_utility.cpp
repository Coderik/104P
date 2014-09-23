/*
 * io_utility.cpp
 *
 *  Created on: Aug 27, 2013
 *      Author: Vadim Fedorov
 */

#include "io_utility.h"

/**
 * Reads image in PGM format from file with provided path\name
 * to the object of 'Image' class
 */
Image<float>* IOUtility::read_pgm_image(const string &name)
{
	/* open file */
	// COMPATIBILITY: for win 'rb' file mode instead of just 'r'
	FILE *f = fopen(name.data(),"rb");
	if( f == NULL ) {
		return 0;
	}

	/* read header */
	bool isBinary = false;
	int c, x_size,y_size,depth;

	if ( fgetc(f) != 'P' ) {
		fclose(f);
		return 0;
	}

	if( (c=fgetc(f)) == '2' ) {
		isBinary = false;
	} else if ( c == '5' ) {
		isBinary = true;
	} else {
		fclose(f);
		return 0;
	}

	skip_spaces_and_comments(f);
	fscanf(f,"%d",&x_size);
	skip_spaces_and_comments(f);
	fscanf(f,"%d",&y_size);
	skip_spaces_and_comments(f);
	fscanf(f,"%d",&depth);

	/* get memory */
	Image<float> *image = new Image<float>(x_size, y_size);

	/* read data */
	skip_spaces_and_comments(f);
	int value;
	for(int y=0;y<y_size;y++) {
		for(int x=0;x<x_size;x++) {
			value = isBinary ? fgetc(f) : get_number(f);
			image->set_value(x,y,value);
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
void IOUtility::write_pgm_image(const string &name, Image<float> *image)
{
	/* open file */
	FILE *f = fopen(name.data(),"wb");

	/* write header */
	putc('P', f);
	putc('5', f);

	/* write attributes */
	int x_size = image->get_size_x();
	int y_size = image->get_size_y();
	fprintf(f, "\n%d %d\n%d\n", x_size, y_size, 255);

	/* write data */
	char value;
	for (int y = 0;y < y_size;y++) {
		for (int x = 0;x < x_size;x++) {
			value = (char)image->get_value(x,y);
			putc(value, f);
		}
	}

	/* close file */
	fclose(f);
}


Image<float>* IOUtility::read_mono_image(const string &name)
{
	int width, height;

	float *image_data = iio_read_image_float(name.data(), &width, &height);

	if (!image_data) {
		fprintf(stderr, "read_mono_image: cannot read %s\n", name.data());
		return 0;
	}

	Image<float> *image = new Image<float>(width, height);

	// copy from image_data to Image<float>
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			int index = (y * width + x);
			float value = image_data[index];
			image->set_value(x, y, value);
		}
	}

	free(image_data);

	return image;
}


void IOUtility::write_mono_image(const string &name, Image<float> *image)
{
	int width = image->get_size_x();
	int height = image->get_size_y();

	float *image_data = new float[width * height];

	// copy from Image<float> to image_data
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			int index = (y * width + x);
			image_data[index] = image->get_value(x, y);
		}
	}

	iio_save_image_float(const_cast<char* >(name.data()), image_data, width, height);
	delete[] image_data;
}


void IOUtility::write_float_image(const string &name, Image<float> *image)
{
	iio_save_image_float_split(const_cast<char*>(name.data()), image->get_raw_data(), image->get_size_x(), image->get_size_y(), 1);
}


string IOUtility::compose_file_name(const string &name, int index, const string &extension)
{
	stringstream stream;
	stream << name << "_" << setfill('0') << setw(3) << index << "." << extension;
	string file_name = stream.str();
	return file_name;
}


string IOUtility::compose_file_name(const string &name, int index, int index2, const string &extension)
{
	stringstream stream;
	stream << name << "_" << index << "_" << setw(3) << setfill('0') << index2 << "." << extension;
	string file_name = stream.str();
	return file_name;
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




