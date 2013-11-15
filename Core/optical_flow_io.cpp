/*
 * optical_flow_io.cpp
 *
 *  Created on: Jan 25, 2013
 *      Author: Vadim Fedorov
 */

#include "optical_flow_io.h"

int MIN_VALUE = -32768;

OFStatus check_optical_flow(const std::string &file_name, int size_x, int size_y, int chunks_count)
{
	std::ifstream file(file_name.data(), std::ios::in | std::ios::binary);
	if (!file)
		return STATUS_NO_FILE;

	OFHeader header;

	file.read((char *) &header, sizeof(struct OFHeader));

	if (!check_header(header) ||
		header.size_x != size_x ||
		header.size_y != size_y ||
		header.chunks_count != chunks_count) {
		file.close();
		return STATUS_NOT_VALID;
	}

	file.close();
	return STATUS_OK;
}


void update_or_overwrite_flow(const std::string &file_name, const float *flow_x, const float *flow_y, int size_x, int size_y, int chunk_id, int chunks_count)
{
	std::fstream file(file_name.data(), std::ios::in | std::ios::out | std::ios::binary);

	// If file is valid, update it. Otherwise, rewrite it.
	if (file) {
		OFHeader header;
		file.read((char *) &header, sizeof(struct OFHeader));
		if (check_header(header) &&
			header.size_x == size_x &&
			header.size_y == size_y &&
			header.chunks_count == chunks_count) {
			// Update file
			update_flow(file, flow_x, flow_y, size_x, size_y, chunk_id, chunks_count);
			file.close();
			return;
		} else {
			file.close();
		}
	}

	// Rewrite file
	overwrite_flow(file_name, flow_x, flow_y, size_x, size_y, chunk_id, chunks_count);
}


void update_or_overwrite_flow(const std::string &file_name, OpticalFlowContainer &flow, int chunk_id, int chunks_count)
{
	update_or_overwrite_flow(file_name, flow.get_flow_x(), flow.get_flow_y(), flow.get_size_x(), flow.get_size_y(), chunk_id, chunks_count);
}


bool read_flow(const std::string &file_name, float *&flow_x, float *&flow_y, int &size_x, int &size_y, int chunk_id)
{
	std::ifstream file(file_name.data(), std::ios::in | std::ios::binary);
	if (!file)
		return false;

	OFHeader header;

	file.read((char *) &header, sizeof(struct OFHeader));
	int chunks_count = header.chunks_count;

	if (!check_header(header) || chunk_id >= chunks_count) {
		file.close();
		return false;
	}

	// Read description array
	int description[chunks_count];
	file.read((char *) &description, sizeof(description));

	// Look for position of chunk with current id of first empty chunk in description array
	int position = -1;
	for (int i = 0; i < chunks_count; i++) {
		if (description[i] == chunk_id || description[i] == MIN_VALUE) {
			position = i;
			break;
		}
	}

	if (position == -1 || description[position] == MIN_VALUE) {
		file.close();
		return false;
	}

	size_x = header.size_x;
	size_y = header.size_y;
	flow_x = new float[size_x * size_y];
	flow_y = new float[size_x * size_y];

	// Read chunk
	int offset = sizeof(OFHeader) + sizeof(description) + position * 2 * sizeof(float) * size_x * size_y;
	file.seekg(offset, std::ios::beg);	//TODO: catch error 'position that could not be reached'
	file.read((char *) flow_x, sizeof(float) * size_x * size_y);
	file.read((char *) flow_y, sizeof(float) * size_x * size_y);

	file.close();
	return true;
}

/***************************************************************************************************************
 * Reads optical flow data for the whole (forward or backward) direction.
 * NOTE:
 * 1. If out_optical_flow_list is empty, it will be initialized with elements of class
 * OpticalFlowContainer to accommodate all readed chunks.
 * 2. If out_optical_flow_list is not empty, but its size differs from the expected one (chunks count / 2),
 * it will be treated as an incorrect input and 'false' will be returned.
 * 3. Already existing elements of out_optical_flow_list (possibly of classes derived from OpticalFlowContainer)
 * are not overwritten and are used to store readed flow data.
 * Missing elements are initialized as objects of class OpticalFlowContainer.
 */
bool read_whole_direction_data(const std::string &file_name, bool forward_direction, std::vector<OpticalFlowContainer*> &out_optical_flow_list)
{
	std::ifstream file(file_name.data(), std::ios::in | std::ios::binary);
	if (!file)
		return false;

	OFHeader header;

	file.read((char *) &header, sizeof(struct OFHeader));

	int size_x = header.size_x;
	int size_y = header.size_y;
	int chunks_count = header.chunks_count;
	int list_expected_size = (int)(chunks_count / 2);

	if (!check_header(header) ||
			(out_optical_flow_list.size() != 0 && out_optical_flow_list.size() != list_expected_size)) {
		file.close();
		return false;
	}

	// if out_optical_flow_list is empty, initialize it
	if (out_optical_flow_list.size() == 0) {
		out_optical_flow_list.reserve(list_expected_size);
		for (int i = 0; i < list_expected_size; i++) {
			out_optical_flow_list.push_back(new OpticalFlowContainer());
		}
	}

	// Read description array
	int description[chunks_count];
	file.read((char *) &description, sizeof(description));

	float *flow_x, *flow_y;
	for (int i = 0; i < chunks_count; i++) {
		int index = -1;
		if (forward_direction && description[i] >= 0) {
			index = description[i];
		} else if (!forward_direction && description[i] < 0 && description[i] > MIN_VALUE ) {
			index = -description[i] - 1;
		}

		if (index >= 0 && index < out_optical_flow_list.size()) {
			flow_x = new float[size_x * size_y];
			flow_y = new float[size_x * size_y];

			// Read a chunk
			int offset = sizeof(OFHeader) + sizeof(description) + i * 2 * sizeof(float) * size_x * size_y;
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
bool check_header(OFHeader header)
{
	return header.signature[0] == 'O' && header.signature[1] == 'F' && header.version == 2;
}


void update_flow(std::fstream &file, const float *flow_x, const float *flow_y, int size_x, int size_y, int chunk_id, int chunks_count)
{
	// Read description array
	int description[chunks_count];
	int offset = sizeof(OFHeader);
	file.seekg(offset, std::ios::beg);
	file.read((char *) &description, sizeof(description));

	// Look for position of chunk with current id of first empty chunk in description array
	int position = -1;
	for (int i = 0; i < chunks_count; i++) {
		if (description[i] == chunk_id || description[i] == MIN_VALUE) {
			position = i;
			break;
		}
	}

	if (position == -1) {
		// TODO: handle error
	}

	if (description[position] == MIN_VALUE) {
		// Update desciption
		description[position] = chunk_id;
		offset = sizeof(OFHeader);
		file.seekp(offset, std::ios::beg);
		file.write((char *) &description, sizeof(description));
	}

	// Write chunk
	offset = sizeof(OFHeader) + sizeof(description) + position * 2 * sizeof(float) * size_x * size_y;
	file.seekp(offset, std::ios::beg);
	file.write((char *) flow_x, sizeof(float) * size_x * size_y);
	file.write((char *) flow_y, sizeof(float) * size_x * size_y);
}


void overwrite_flow(const std::string &file_name, const float *flow_x, const float *flow_y, int size_x, int size_y, int chunk_id, int chunks_count)
{
	std::ofstream file(file_name.data(), std::ios::out | std::ios::binary);

	OFHeader header;
	header.signature[0] = 'O';
	header.signature[1] = 'F';
	header.version = 2;
	header.size_x = size_x;
	header.size_y = size_y;
	header.chunks_count = chunks_count;

	file.write((char *) &header, sizeof(struct OFHeader));

	int description[chunks_count];
	description[0] = chunk_id;
	for (int i = 1; i < chunks_count; i++) {
		description[i] = MIN_VALUE;
	}

	file.write((char *) &description, sizeof(description));

	file.write((char *) flow_x, sizeof(float) * size_x * size_y);
	file.write((char *) flow_y, sizeof(float) * size_x * size_y);

	file.close();
}


