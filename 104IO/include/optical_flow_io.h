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
#include <vector>

#include "image.h"

using namespace std;

/**
 * Contains methods for reading and writings [optical] flow.
 */
class OpticalFlowIO
{
public:
	enum OFStatus
	{
		STATUS_OK,
		STATUS_NO_FILE,
		STATUS_NOT_VALID,
		STATUS_LEGACY_FORMAT
	};

	static OFStatus check_optical_flow(const string &file_name, int size_x, int size_y, int chunks_count);
	static void update_or_overwrite_flow(const string &file_name, const float *flow, int size_x, int size_y, int chunk_id, int chunks_count);
	static void update_or_overwrite_flow(const string &file_name, const ImageFx<float> &flow, int chunk_id, int chunks_count);
	static bool read_flow(const string &file_name, float *&flow, int &size_x, int &size_y, int chunk_id);
	static Image<float> read_flow(const string &file_name, int chunk_id);
	// TODO: add method for reading all chunks at a time. To do so, it might be necessary
	// to move OpticalFlow class inside the library and use vector<OpticalFlow> to return all flows.
	// In that case it could be a good idea to inherit from the OpticalFlow in UI project and
	// put all methods for drawing views of optical flow in that child class.
	static vector<Image<float> > read_whole_direction_data(const string &file_name, bool forward_direction);
private:
	struct OFHeader
	{
		char signature[2];
		int version, size_x, size_y, chunks_count;
	};

	static bool check_header(OFHeader header);
	static void update_flow(fstream &file, const float *flow, int size_x, int size_y, int chunk_id, int chunks_count);
	static void overwrite_flow(const string &file_name, const float *flow, int size_x, int size_y, int chunk_id, int chunks_count);
};


#endif /* OPTICAL_FLOW_IO_H_ */
