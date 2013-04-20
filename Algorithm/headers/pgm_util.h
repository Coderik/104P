/*
 * pgm.h
 *
 *  Created on: Nov 5, 2012
 *      Author: Vadim Fedorov
 */

#ifndef PGM_H_
#define PGM_H_

#include <string>
#include "image.h"

using namespace std;

Image<float>* ReadPgmImage(string *name);
void WritePgmImage(string *name, Image<float> *image);

#endif /* PGM_H_ */
