/*
 * gaussian_kernel.h
 *
 *  Created on: Jun 26, 2013
 *      Author: Vadim Fedorov
 */

#ifndef GAUSSIAN_KERNEL_H_
#define GAUSSIAN_KERNEL_H_

#include <algorithm>
#include <math.h>

using namespace std;

class GaussianKernel
{
public:
	GaussianKernel(unsigned int size_x, unsigned int size_y, unsigned int size_z);
	~GaussianKernel();

	float get(unsigned int x, unsigned int y, unsigned int z);
	float get(int x, int y, int z);

private:
	static const float STEP = 0.1;
	unsigned int _size_x, _size_y, _size_z;
	float *_values;

	void build_kernel();
	inline int get_index(int x, int y, int z) const;
};


#endif /* GAUSSIAN_KERNEL_H_ */
