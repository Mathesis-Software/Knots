#include "experimental.h"
#include "../knot.h"

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

}

double Experimental2::compute() {
	double value = 0.0;

	std::size_t i1, i2;
	int o;

	const auto points = this->knot.points();
	// Вычисляем заранее касательные векторы.
	double **tangs = new double*[points.size()];
	for (i1 = 0; i1 < points.size(); i1++) {
		tangs[i1] = new double[3];
		tangs[i1][0] = points[points.next(i1)].x - points[i1].x;
		tangs[i1][1] = points[points.next(i1)].y - points[i1].y;
		tangs[i1][2] = points[points.next(i1)].z - points[i1].z;
	}

	// Вычисляем ``гауссовы произведения''.
	double chord[3], chord_len;
	double **gauss = new double*[points.size()];

	for (i1 = 0; i1 < points.size(); i1++) {
		gauss[i1] = new double[points.size()];
		gauss[i1][i1] = 0.0;
		for (i2 = 0; i2 < i1; i2++) {
			chord[0] = ( points[i1].x + points[points.next(i1)].x -
										points[i2].x - points[points.next(i2)].x ) / 2;
			chord[1] = ( points[i1].y + points[points.next(i1)].y -
										points[i2].y - points[points.next(i2)].y ) / 2;
			chord[2] = ( points[i1].z + points[points.next(i1)].z -
										points[i2].z - points[points.next(i2)].z ) / 2;
			chord_len = sqrt (vector_square (chord));
			gauss[i1][i2] = det (tangs[i1], tangs[i2], chord) /
												(chord_len * chord_len * chord_len);
			gauss[i2][i1] = gauss[i1][i2];
		}
	}

	// Вычисляем суммы ``гауссовых произведений''.

	// В gauss_sum[i1][i2] находится сумма ``гауссовых произведений''
	// для всех хорд с началом в i1 и концом от points.next(i1) до i2.
	double **gauss_sum = new double*[points.size()];

	for (i1 = 0; i1 < points.size(); i1++) {
		gauss_sum[i1] = new double[points.size()];
		gauss_sum[i1][i1] = 0.0;
		for (i2 = points.next(i1); i2 != i1; i2 = points.next(i2))
			gauss_sum[i1][i2] = gauss_sum[i1][points.prev(i2)] + gauss[i1][i2];
	}

	double tmp, tmp2;
	for (i1 = 0; i1 < points.size(); i1++) {
		tmp = 0.0;
		for (i2 = points.next(points.next(i1)); i2 != i1; i2 = points.next(i2)) {
			tmp += gauss_sum[points.prev(i2)][points.prev(i1)] -
						 gauss_sum[i2][points.prev(points.prev(i2))] + gauss_sum[i2][i1];
			tmp2 = gauss[i1][i2];
			for (o = 1; o < order; o++)
				tmp2 *= tmp / 16;
			value += tmp2;
		}
	}

	// Удаляем заранее вычисленные вспомогательные значения.
	for (i1 = 0; i1 < points.size(); i1++) {
		delete[] tangs[i1];
		delete[] gauss[i1];
		delete[] gauss_sum[i1];
	}
	delete[] tangs;
	delete[] gauss;
	delete[] gauss_sum;

	return value / (4 * M_PI * M_PI);
}

}}}
