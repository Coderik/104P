/**
 * Copyright (C) 2016, Vadim Fedorov <coderiks@gmail.com>
 *
 * This program is free software: you can use, modify and/or
 * redistribute it under the terms of the simplified BSD
 * License. You should have received a copy of this license along
 * this program. If not, see
 * <http://www.opensource.org/licenses/bsd-license.html>.
 */

/// Created on: Jan 25, 2013

#include "optical_flow_io.h"

int MIN_VALUE = -32768;

OpticalFlowIO::OFStatus OpticalFlowIO::check_optical_flow(const std::string &file_name, int size_x, int size_y, int chunks_count)
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


void OpticalFlowIO::update_or_overwrite_flow(const std::string &file_name, const float *flow, int size_x, int size_y, int chunk_id, int chunks_count)
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
			update_flow(file, flow, size_x, size_y, chunk_id, chunks_count);
			file.close();
			return;
		} else {
			file.close();
		}
	}

	// Rewrite file
	overwrite_flow(file_name, flow, size_x, size_y, chunk_id, chunks_count);
}


void OpticalFlowIO::update_or_overwrite_flow(const std::string &file_name, const ImageFx<float> &flow, int chunk_id, int chunks_count)
{
	if (flow.number_of_channels() == 2) {
		update_or_overwrite_flow(file_name, flow.raw(), flow.size_x(), flow.size_y(), chunk_id, chunks_count);
	}
}


bool OpticalFlowIO::read_flow(const std::string &file_name, float *&flow, int &size_x, int &size_y, int chunk_id)
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
	flow = new float[size_x * size_y * 2];

	// Read chunk
	int offset = sizeof(OFHeader) + sizeof(description) + position * 2 * sizeof(float) * size_x * size_y;
	file.seekg(offset, std::ios::beg);	//TODO: catch error 'position that could not be reached'
	file.read((char *) flow, sizeof(float) * size_x * size_y * 2);

	file.close();
	return true;
}


Image<float> OpticalFlowIO::read_flow(const std::string &file_name, int chunk_id)
{
	std::ifstream file(file_name.data(), std::ios::in | std::ios::binary);
	if (!file)
		return Image<float>();

	OFHeader header;

	file.read((char *) &header, sizeof(struct OFHeader));
	int chunks_count = header.chunks_count;

	if (!check_header(header) || chunk_id >= chunks_count) {
		file.close();
		return Image<float>();
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
		return Image<float>();
	}

	int size_x = header.size_x;
	int size_y = header.size_y;
	Image<float> flow(size_x, size_y, 2, 0.0f);

	// Read chunk
	int offset = sizeof(OFHeader) + sizeof(description) + position * 2 * sizeof(float) * size_x * size_y;
	file.seekg(offset, std::ios::beg);	//TODO: catch error 'position that could not be reached'
	file.read((char *) flow.raw(), sizeof(float) * size_x * size_y * 2);

	file.close();
	return flow;
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
std::vector<Image<float> > OpticalFlowIO::read_whole_direction_data(const std::string &file_name, bool forward_direction)
{
	std::ifstream file(file_name.data(), std::ios::in | std::ios::binary);
	if (!file) {
		return std::vector<Image<float> >();
	}

	OFHeader header;

	file.read((char *) &header, sizeof(struct OFHeader));

	int size_x = header.size_x;
	int size_y = header.size_y;
	int chunks_count = header.chunks_count;

	if (!check_header(header)) {
		file.close();
		return std::vector<Image<float> >();
	}

	std::vector<Image<float> > optical_flow_list(chunks_count / 2);

	// Read description array
	int description[chunks_count];
	file.read((char *) &description, sizeof(description));

	for (int i = 0; i < chunks_count; i++) {
		int index = -1;
		if (forward_direction && description[i] >= 0) {
			index = description[i];
		} else if (!forward_direction && description[i] < 0 && description[i] > MIN_VALUE ) {
			index = -description[i] - 1;
		}

		if (index >= 0 && index < header.chunks_count) {
			Image<float> flow(size_x, size_y, 2, 0.0f);

			// Read a chunk
			int offset = sizeof(OFHeader) + sizeof(description) + i * 2 * sizeof(float) * size_x * size_y;
			file.seekg(offset, std::ios::beg);	//TODO: catch error 'position that could not be reached'
			file.read((char *) flow.raw(), sizeof(float) * size_x * size_y * 2);

			// Put chunk in the list
			optical_flow_list[index] = flow;
		}
	}

	file.close();
	return optical_flow_list;
}


/* private */
bool OpticalFlowIO::check_header(OFHeader header)
{
	return header.signature[0] == 'O' && header.signature[1] == 'F' && header.version == 3;
}


void OpticalFlowIO::update_flow(std::fstream &file, const float *flow, int size_x, int size_y, int chunk_id, int chunks_count)
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
	file.write((char *) flow, sizeof(float) * size_x * size_y * 2);
}


void OpticalFlowIO::overwrite_flow(const std::string &file_name, const float *flow, int size_x, int size_y, int chunk_id, int chunks_count)
{
	std::ofstream file(file_name.data(), std::ios::out | std::ios::binary);

	OFHeader header;
	header.signature[0] = 'O';
	header.signature[1] = 'F';
	header.version = 3;
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

	file.write((char *) flow, sizeof(float) * size_x * size_y * 2);

	file.close();
}


