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

// TODO: [?] generalize kernels (might even have common ancestor with Image)
// TODO: make template class: template<int dim>

class GaussianKernel
{
public:
	GaussianKernel(unsigned int size_x, unsigned int size_y, unsigned int size_z, float sigma_scale = DEFAULT_SIGMA_SCALE);
	GaussianKernel(unsigned int size_x, unsigned int size_y, unsigned int size_z, float sigma_x, float sigma_y, float sigma_z);
	~GaussianKernel();

	float get(unsigned int x, unsigned int y, unsigned int z);
	float get(int x, int y, int z);

	const float* get_raw() const;

private:
	static const float DEFAULT_SIGMA_SCALE = 6;
	static const float STEP = 0.01;
	unsigned int _size_x, _size_y, _size_z;
	float _sigma_x, _sigma_y, _sigma_z;
	float *_values;

	void build_kernel();
	inline int get_index(int x, int y, int z) const;
};


#endif /* GAUSSIAN_KERNEL_H_ */
