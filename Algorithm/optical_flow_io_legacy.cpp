/*
 * optical_flow_io_legacy.cpp
 *
 *  Created on: Feb 25, 2013
 *      Author: Vadim Fedorov
 */

#include "headers/optical_flow_io_legacy.h"

OFStatus check_optical_flow_legacy(const std::string &file_name, int size_x, int size_y, int frames_count)
{
	std::ifstream file(file_name.data(), std::ios::in | std::ios::binary);
	if (!file)
		return STATUS_NO_FILE;

	OFHeader_v_1 header;

	file.read((char *) &header, sizeof(struct OFHeader_v_1));

	if (!check_header_legacy(header) ||
		header.size_x != size_x ||
		header.size_y != size_y ||
		header.chunks_count != frames_count - 1) {
		file.close();
		return STATUS_NOT_VALID;
	}

	file.close();
	return STATUS_LEGACY_FORMAT;
}


bool read_forward_optical_flow_legacy(const std::string &file_name, std::vector<OpticalFlowContainer*> &out_optical_flow_list)
{
	std::ifstream file(file_name.data(), std::ios::in | std::ios::binary);
	if (!file)
		return false;

	OFHeader_v_1 header;

	file.read((char *) &header, sizeof(struct OFHeader_v_1));

	int size_x = header.size_x;
	int size_y = header.size_y;
	int chunks_count = header.chunks_count;

	if (!check_header_legacy(header) ||
			(out_optical_flow_list.size() != 0 && out_optical_flow_list.size() != chunks_count)) {
		file.close();
		return false;
	}

	// if out_optical_flow_list is empty, initialize it
	if (out_optical_flow_list.size() == 0) {
		out_optical_flow_list.reserve(chunks_count);
		for (int i = 0; i < chunks_count; i++) {
			out_optical_flow_list.push_back(new OpticalFlowContainer());
		}
	}

	// Read description array
	int description[chunks_count];
	file.read((char *) &description, sizeof(description));

	float *flow_x, *flow_y;
	for (int i = 0; i < chunks_count; i++) {
		int index = description[i];

		if (index >= 0 && (unsigned)index < out_optical_flow_list.size()) {
			flow_x = new float[size_x * size_y];
			flow_y = new float[size_x * size_y];

			// Read a chunk
			int offset = sizeof(OFHeader_v_1) + sizeof(description) + i * 2 * sizeof(float) * size_x * size_y;
			file.seekg(offset, std::ios::beg);	//TODO: catch error 'position that could not be reached'
			file.read((char *) flow_x, sizeof(float) * size_x * size_y);
			file.read((char *) flow_y, sizeof(float) * size_x * size_y);

			// Put chunk in the list
			if (!out_optical_flow_list[index]) {
				out_optical_flow_list[index] = new OpticalFlowContainer();
			} else if (out_optical_flow_list[index]->contains_data()) {
				out_optical_flow_list[index]->clear();
			}
			out_optical_flow_list[index]->set_flow(flow_x, flow_y, size_x, size_y);
		}
	}

	file.close();
	return true;
}


/* private */
bool check_header_legacy(OFHeader_v_1 header)
{
	return header.signature[0] == 'O' && header.signature[1] == 'F' && header.version == 1;
}


