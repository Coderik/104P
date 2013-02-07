/*
 * optical_flow_io.cpp
 *
 *  Created on: Jan 25, 2013
 *      Author: upf
 */

#include "headers/optical_flow_io.h"

bool check_optical_flow(const std::string &file_name, int size_x, int size_y, int chunks_count)
{
	std::ifstream file(file_name.data(), std::ios::in | std::ios::binary);
	if (!file)
		return false;

	OFHeader header;

	file.read((char *) &header, sizeof(struct OFHeader));

	if (!check_header(header) ||
		header.size_x != size_x ||
		header.size_y != size_y ||
		header.chunks_count != chunks_count) {
		file.close();
		return false;
	}

	file.close();
	return true;
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
		if (description[i] == chunk_id || description[i] == -1) {
			position = i;
			break;
		}
	}

	if (position == -1 || description[position] == -1) {
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


/* private */
bool check_header(OFHeader header)
{
	return header.signature[0] == 'O' && header.signature[1] == 'F' && header.version == 1;
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
		if (description[i] == chunk_id || description[i] == -1) {
			position = i;
			break;
		}
	}

	if (position == -1) {
		// TODO: handle error
	}

	if (description[position] == -1) {
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
	header.version = 1;
	header.size_x = size_x;
	header.size_y = size_y;
	header.chunks_count = chunks_count;

	file.write((char *) &header, sizeof(struct OFHeader));

	int description[chunks_count];
	description[0] = chunk_id;
	for (int i = 1; i < chunks_count; i++) {
		description[i] = -1;
	}

	file.write((char *) &description, sizeof(description));

	file.write((char *) flow_x, sizeof(float) * size_x * size_y);
	file.write((char *) flow_y, sizeof(float) * size_x * size_y);

	file.close();
}


