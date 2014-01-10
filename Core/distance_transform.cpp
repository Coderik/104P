/*
 * distance_transform.cpp
 *
 *  Created on: Jul 11, 2013
 *      Author: Vadim Fedorov
 */

#include "distance_transform.h"

Image<float>* DistanceTransform::calculate(ImageMask &mask, DistanceEnum distance_type)
{
	return calculate_internal(mask, distance_type, true);
}


Image<float>* DistanceTransform::calculate_inverse(ImageMask &mask, DistanceEnum distance_type)
{
	return calculate_internal(mask, distance_type, false);
}

/* Private */

Image<float>* DistanceTransform::calculate_internal(ImageMask &mask, DistanceEnum distance_type, bool direct)
{
	// NOTE: it's hard to give meaningful names to the variables,
	// therefore, most of them are named like in the paper.

	// allocate buffer to store partial results ('G' function's values in the paper)
	Image<float> g(mask.get_size_x(), mask.get_size_y());

	// allocate memory for result
	Image<float> *distances = new Image<float>(mask.get_size_x(), mask.get_size_y());

	// NOTE: infinity is replaced by the maximum possible distance
	const float INF = mask.get_size_x() + mask.get_size_y();

	// first phase
	for (int x = 0; x < mask.get_size_x(); x++) {
		// scan 1 (top to bottom)
		if (mask.get_value(x, 0) != direct) {				// NOTE: A != B is equivalent to A XOR B
			g.set_value(x, 0, 0.0);
		} else {
			g.set_value(x, 0, INF);
		}

		for (int y = 1; y < mask.get_size_y(); y++) {
			if (mask.get_value(x, y) != direct) {			// NOTE: A != B is equivalent to A XOR B
				g.set_value(x, y, 0.0);
			} else {
				g.set_value(x, y, g.get_value(x, y - 1) + 1.0);
			}
		}

		// scan 2 (bottom to top)
		for (int y = mask.get_size_y() - 2; y >= 0; y--) {
			if (g.get_value(x, y + 1) < g.get_value(x, y)) {
				g.set_value(x, y, g.get_value(x, y + 1) + 1.0);
			}
		}
	}

	// second phase
	vector<int> s(mask.get_size_x());
	vector<int> t(mask.get_size_x());
	for (int y = 0; y < mask.get_size_y(); y++) {
		int q = 0;
		s[0] = 0;
		t[0] = 0;

		// scan 3 (left to right)
		for (int u = 1; u < mask.get_size_x(); u++) {
			while (q >= 0 && f(t[q], s[q], g.get_value(s[q], y), distance_type) > f(t[q], u, g.get_value(u, y), distance_type) ) {
				q--;
			}

			if (q < 0) {
				q = 0;
				s[0] = u;
			} else {
				int w = 1 + sep(s[q], u, g.get_value(s[q], y), g.get_value(u, y), INF, distance_type);
				if (w < mask.get_size_x()) {
					q++;
					s[q] = u;
					t[q] = w;
				}
			}
		}

		// scan 4 (right to left)
		for (int u = mask.get_size_x() - 1; u >= 0; u--) {
			distances->set_value(u, y, f(u, s[q], g.get_value(s[q], y), distance_type) );
			if (u == t[q]) {
				q--;
			}
		}

		s.clear();
		t.clear();
	}

	return distances;
}


inline float DistanceTransform::f(int x, int x_i, float g_i, DistanceEnum distance_type)
{
	float result = 0.0;
	switch (distance_type) {
		case Distance::Euclidean :
			result = sqrt(pow((float)(x - x_i), 2) + pow(g_i, 2));
			break;
		case Distance::Manhattan :
			result = abs(x - x_i) + g_i;
			break;
		case Distance::chessboard :
			result = max((float)abs(x - x_i), g_i);
			break;
	}

	return result;
}


inline int DistanceTransform::sep(int i, int u, float g_i, float g_u, int inf, DistanceEnum distance_type)
{
	int result = 0;

	// TODO: implement
	switch (distance_type) {
		case Distance::Euclidean :
			result = (pow((float)u, 2) - pow((float)i, 2) + pow(g_u, 2) - pow(g_i, 2)) / (2 * (u - i));
			break;
		case Distance::Manhattan :
			if (g_u >= g_i + u - i) {
				result = inf;
			} else if (g_i > g_u + u - i) {
				result = -inf;
			} else {
				result = (g_u - g_i + u + i) / 2;
			}
			break;
		case Distance::chessboard :
			if (g_i <= g_u) {
				result = max((int)(i + g_u), (i + u) / 2);
			} else {
				result = min((int)(u - g_i), (i + u) / 2);
			}
			break;
	}

	return result;
}


