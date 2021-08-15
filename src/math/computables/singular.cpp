#include "experimental.h"
#include "../knot/Knot.h"

namespace KE { namespace ThreeD { namespace Computables {

namespace {

double det(const double *v1, const double *v2, const double *v3) {
	return
			v1[0] * (v2[1] * v3[2] - v2[2] * v3[1])
		+ v1[1] * (v2[2] * v3[0] - v2[0] * v3[2])
		+ v1[2] * (v2[0] * v3[1] - v2[1] * v3[0]);
}

double vector_square(const double *v) {
	return v[0] * v[0] + v[1] * v[1] + v[2] * v[2];
}

double scalar_product(const double *v1, const double *v2) {
	return v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2];
}

bool inside(const double *a, const double *b, const double *c, const double *d, const double *x) {
	const bool sign = det(a, b, x) > 0;
	return (det(b, c, x) > 0) == sign && (det(c, d, x) > 0) == sign && (det(d, a, x) > 0) == sign;
}

/*static int intersected (const double *a, const double *b, const double *c, const double *d) {
	if (det(a, b, c) * det(a, b, d) > 0)
		return 0;
	if (det(a, c, d) * det(b, c, d) > 0)
		return 0;
	return 1;
}*/

}

double Singular::compute() {
	double value = 0.0;

	std::size_t i1, i2, i3, i4;

	const auto points = this->knot.snapshot();
	// Вычисляем заранее хорды.
	double chord_len;
	double ***chord = new double**[points.size()];

	for (i1 = 0; i1 < points.size(); i1++) {
		chord[i1] = new double*[points.size()];
		for (i2 = 0; i2 < points.size(); i2++)
			chord[i1][i2] = new double[3];

		for (i2 = 0; i2 < i1; i2++) {
			chord[i1][i2][0] = points[i1].x - points[i2].x;
			chord[i1][i2][1] = points[i1].y - points[i2].y;
			chord[i1][i2][2] = points[i1].z - points[i2].z;
			chord_len = sqrt (vector_square (chord[i1][i2]));
			chord[i1][i2][0] /= chord_len;
			chord[i1][i2][1] /= chord_len;
			chord[i1][i2][2] /= chord_len;
			chord[i2][i1][0] = - chord[i1][i2][0];
			chord[i2][i1][1] = - chord[i1][i2][1];
			chord[i2][i1][2] = - chord[i1][i2][2];
		}
	}

	double curr_cos, min_cos = 1.0;
	for (i1 = 0; i1 < points.size(); i1++)
		for (i2 = points.next(i1); i2 != points.prev(i1); i2 = points.next(i2)) {
			curr_cos = fabs (scalar_product (chord[i1][i2], chord[i1][points.next(i2)]));
			if (min_cos > curr_cos)
				min_cos = curr_cos;
		}
	min_cos = 4 * min_cos - 3;

	for (i1 = 0; i1 < points.size() - 3; i1++)
		for (i2 = i1 + 1; i2 < points.size() - 2; i2++)
			for (i3 = i2 + 1; i3 < points.size() - 1; i3++)
				for (i4 = i3 + 1; i4 < points.size(); i4++) {
		if (fabs (scalar_product (chord[i1][i3], chord[i2][i4])) < min_cos)
			continue;
//					value += 1;
//		if (intersected (chord[i1][i3],
//										 chord[i1][points.next(i3)],
//										 chord[i2][i4],
//				 chord[i2][points.next(i4)])
//						value += 1;
		if (inside (chord[i1][i3],
								chord[i1][points.next(i3)],
								chord[points.next(i1)][points.next(i3)],
					chord[points.next(i1)][i3],
								chord[i2][i4]))
						value += 1;
		if (inside (chord[i1][i3],
								chord[i1][points.next(i3)],
								chord[points.next(i1)][points.next(i3)],
					chord[points.next(i1)][i3],
								chord[i2][points.next(i4)]))
						value += 1;
		if (inside (chord[i1][i3],
								chord[i1][points.next(i3)],
								chord[points.next(i1)][points.next(i3)],
					chord[points.next(i1)][i3],
								chord[points.next(i2)][i4]))
						value += 1;
		if (inside (chord[i1][i3],
								chord[i1][points.next(i3)],
								chord[points.next(i1)][points.next(i3)],
					chord[points.next(i1)][i3],
								chord[points.next(i2)][points.next(i4)]))
						value += 1;
		if (inside (chord[i2][i4],
								chord[i2][points.next(i4)],
								chord[points.next(i2)][points.next(i4)],
					chord[points.next(i2)][i4],
								chord[i1][i3]))
						value += 1;
		if (inside (chord[i2][i4],
								chord[i2][points.next(i4)],
								chord[points.next(i2)][points.next(i4)],
					chord[points.next(i2)][i4],
								chord[i1][points.next(i3)]))
						value += 1;
		if (inside (chord[i2][i4],
								chord[i2][points.next(i4)],
								chord[points.next(i2)][points.next(i4)],
					chord[points.next(i2)][i4],
								chord[points.next(i1)][i3]))
						value += 1;
		if (inside (chord[i2][i4],
								chord[i2][points.next(i4)],
								chord[points.next(i2)][points.next(i4)],
					chord[points.next(i2)][i4],
								chord[points.next(i1)][points.next(i3)]))
						value += 1;
				}

	// Удаляем заранее вычисленные вспомогательные значения.
	for (i1 = 0; i1 < points.size(); i1++) {
		for (i2 = 0; i2 < points.size(); i2++)
			delete[] chord[i1][i2];
		delete[] chord[i1];
	}
	delete[] chord;

	return value;
}

}}}
