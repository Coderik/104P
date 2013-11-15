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

Image<float>* ReadPgmImage(const string &name);
void WritePgmImage(const string &name, Image<float> *image);

#endif /* PGM_H_ */
