/*
 * optical_flow_io.h
 *
 *  Created on: Jan 25, 2013
 *      Author: upf
 */

#ifndef OPTICAL_FLOW_IO_H_
#define OPTICAL_FLOW_IO_H_

#include <string>
#include <iostream>
#include <algorithm>
#include <stdio.h>
#include <fstream>

#include "optical_flow_container.h"


struct OFHeader
{
	char signature[2];
	int version, size_x, size_y, chunks_count;
};

extern bool check_optical_flow(const std::string &file_name, int size_x, int size_y, int chunks_count);
extern void update_or_overwrite_flow(const std::string &file_name, const float *flow_x, const float *flow_y, int size_x, int size_y, int chunk_id, int chunks_count);
extern void update_or_overwrite_flow(const std::string &file_name, OpticalFlowContainer &flow, int chunk_id, int chunks_count);
extern bool read_flow(const std::string &file_name, float *&flow_x, float *&flow_y, int &size_x, int &size_y, int chunk_id);
// TODO: add method for reading all chunks at a time. To do so, it might be necessary
// to move OpticalFlow class inside the library and use vector<OpticalFlow> to return all flows.
// In that case it could be a good idea to inherit from the OpticalFlow in UI project and
// put all methods for drawing views of optical flow in that child class.

/* private */
extern bool check_header(OFHeader header);
extern void update_flow(std::fstream &file, const float *flow_x, const float *flow_y, int size_x, int size_y, int chunk_id, int chunks_count);
extern void overwrite_flow(const std::string &file_name, const float *flow_x, const float *flow_y, int size_x, int size_y, int chunk_id, int chunks_count);


#endif /* OPTICAL_FLOW_IO_H_ */
