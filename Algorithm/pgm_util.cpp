/*
 * pgm.cpp
 *
 *  Created on: Nov 5, 2012
 *      Author: Vadim Fedorov
 */

#include <stdlib.h>
#include <stdio.h>
#include <string>

#include "headers/pgm_util.h"


void SkipSpacesAndComments(FILE * f)
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
int GetNumber(FILE * f)
{
  int num, c;

  while(isspace(c=fgetc(f)));
  if(!isdigit(c)) exit(1);
  num = c - '0';
  while( isdigit(c=fgetc(f)) ) num = 10 * num + c - '0';

  return num;
}

/**
 * Reads image in PGM format from file with provided path\name
 * to the object of 'Image' class
 */
Image<float>* ReadPgmImage(string *name)
{
	/* open file */
	// COMPATIBILITY: for win 'rb' file mode instead of just 'r'
	FILE *f = fopen((*name).data(),"rb");
	if( f == NULL ) exit(1);

	/* read header */
	bool isBinary = false;
	int c, x_size,y_size,depth;

	if ( fgetc(f) != 'P' )
		exit(1);

	if( (c=fgetc(f)) == '2' ) {
		isBinary = false;
	} else if ( c == '5' ) {
		isBinary = true;
	} else {
		exit(1);
	}

	SkipSpacesAndComments(f);
	fscanf(f,"%d",&x_size);
	SkipSpacesAndComments(f);
	fscanf(f,"%d",&y_size);
	SkipSpacesAndComments(f);
	fscanf(f,"%d",&depth);

	/* get memory */
	Image<float> *image = new Image<float>(x_size, y_size);

	/* read data */
	SkipSpacesAndComments(f);
	int value;
	for(int y=0;y<y_size;y++) {
		for(int x=0;x<x_size;x++) {
			value = isBinary ? fgetc(f) : GetNumber(f);
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
void WritePgmImage(string *name, Image<float> *image)
{
	/* open file */
	FILE *f = fopen((*name).data(),"w");

	/* write header */
	putc('P', f);
	putc('5', f);

	/* write attributes */
	int x_size = image->GetXSize();
	int y_size = image->GetYSize();
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



