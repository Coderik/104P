/*
 * optical_flow_io_legacy.h
 *
 *  Created on: Feb 25, 2013
 *      Author: Vadim Fedorov
 */

#ifndef OPTICAL_FLOW_IO_LEGACY_H_
#define OPTICAL_FLOW_IO_LEGACY_H_

#include <string>
#include <iostream>
#include <algorithm>
#include <stdio.h>
#include <fstream>
#include <vector>

#include "optical_flow_io.h"

struct OFHeader_v_1
{
	char signature[2];
	int version, size_x, size_y, chunks_count;
};

extern OFStatus check_optical_flow_legacy(const std::string &file_name, int size_x, int size_y, int frames_count);
extern bool read_forward_optical_flow_legacy(const std::string &file_name, std::vector<OpticalFlowContainer*> &out_optical_flow_list);
/* private */
extern bool check_header_legacy(OFHeader_v_1 header);

#endif /* OPTICAL_FLOW_IO_LEGACY_H_ */
