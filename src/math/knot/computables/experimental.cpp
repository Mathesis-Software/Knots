#include "experimental.h"
#include "../Knot.h"

namespace KE { namespace ThreeD { namespace Computables {

namespace {

double vector_square(const double *v) {
	return v[0] * v[0] + v[1] * v[1] + v[2] * v[2];
}

void vector_product(const double *v1, const double *v2, double *m) {
	m[0] = v1[1] * v2[2] - v1[2] * v2[1];
	m[1] = v1[2] * v2[0] - v1[0] * v2[2];
	m[2] = v1[0] * v2[1] - v1[1] * v2[0];
}

}

Experimental::Experimental(const Knot &knot) : Computable(knot, "Experimental") {
}

//double Experimental::compute() {
//	double value = 0.0;
//
//	std::size_t i1, i2, i3, i4;
//
//	// Вычисляем заранее касательные векторы.
//	double **tangs = new double*[points.size()];
//	for (i1 = 0; i1 < points.size(); i1++)
//	{
//		tangs[i1] = new double[3];
//		tangs[i1][0] = points[points.next(i1)].x - points[i1].x;
//		tangs[i1][1] = points[points.next(i1)].y - points[i1].y;
//		tangs[i1][2] = points[points.next(i1)].z - points[i1].z;
//	}
//
//	// Вычисляем ``гауссовы произведения''.
//	double chord_len;
//	double ***chord = new double**[points.size()];
//	double ***chord2 = new double**[points.size()];
//	double tmp[3], tmp2[3];
//
//	for (i1 = 0; i1 < points.size(); i1++)
//	{
//		chord[i1] = new double*[points.size()];
//		chord2[i1] = new double*[points.size()];
//		for (i2 = 0; i2 < points.size(); i2++)
//		{
//			chord[i1][i2] = new double[3];
//			chord2[i1][i2] = new double[3];
//		}
//
//		for (i2 = 0; i2 < i1; i2++)
//		{
//			chord[i1][i2][0] = ( points[i1].x + points[points.next(i1)].x -
//										points[i2].x - points[points.next(i2)].x ) / 2;
//			chord[i1][i2][1] = ( points[i1].y + points[points.next(i1)].y -
//										points[i2].y - points[points.next(i2)].y ) / 2;
//			chord[i1][i2][2] = ( points[i1].z + points[points.next(i1)].z -
//										points[i2].z - points[points.next(i2)].z ) / 2;
//			chord[i2][i1][0] = - chord[i1][i2][0];
//			chord[i2][i1][1] = - chord[i1][i2][1];
//			chord[i2][i1][2] = - chord[i1][i2][2];
//			chord_len = sqrt (vector_square (chord[i1][i2]));
//			chord_len = 1.0 / (chord_len * chord_len * chord_len);
//			vector_product (tangs[i1], tangs[i2], tmp);
//			vector_product (tmp, chord[i1][i2], tmp2);
//			vector_product (tmp2, chord[i1][i2], chord2[i1][i2]);
//			chord2[i1][i2][0] *= chord_len;
//			chord2[i1][i2][1] *= chord_len;
//			chord2[i1][i2][2] *= chord_len;
//			chord2[i2][i1][0] = - chord2[i1][i2][0];
//			chord2[i2][i1][1] = - chord2[i1][i2][1];
//			chord2[i2][i1][2] = - chord2[i1][i2][2];
//		}
//	}
//
//	double normal[3];
//
//	for (i1 = 0; i1 < points.size(); i1++)
//		for (i2 = i1 + 1; i2 < points.size(); i2++)
//			for (i3 = i2 + 1; i3 < points.size(); i3++)
//				for (i4 = i3 + 1; i4 < points.size(); i4++)
//	{
//					vector_product (chord[i1][i3], chord[i2][i4], normal);
//
//					value +=
//			scalar_product (chord2[i1][i3], chord[i2][i4]) *
//			scalar_product (chord2[i2][i4], chord[i1][i3]) /
//			vector_square (normal);
//				}
//
//	// Удаляем заранее вычисленные вспомогательные значения.
//	for (i1 = 0; i1 < points.size(); i1++)
//	{
//		for (i2 = 0; i2 < points.size(); i2++)
//		{
//			delete[] chord[i1][i2];
//			delete[] chord2[i1][i2];
//		}
//
//		delete[] tangs[i1];
//		delete[] chord[i1];
//		delete[] chord2[i1];
//	}
//	delete[] tangs;
//	delete[] chord;
//	delete[] chord2;
//
//	return value / (4 * M_PI * M_PI);
//}

double Experimental::compute() {
	double value = 0.0;

	const auto points = this->knot.points();
	// Вычисляем заранее касательные векторы.
	double **tangs = new double*[points.size()];
	for (std::size_t i1 = 0; i1 < points.size(); i1++) {
		tangs[i1] = new double[3];
		tangs[i1][0] = points[points.next(i1)].x - points[i1].x;
		tangs[i1][1] = points[points.next(i1)].y - points[i1].y;
		tangs[i1][2] = points[points.next(i1)].z - points[i1].z;
	}

	// Вычисляем ``гауссовы произведения''.
	double chord_len;
	double ***chord = new double**[points.size()];
	double ***vector = new double**[points.size()];
	double **sum = new double*[points.size()];

	for (std::size_t i1 = 0; i1 < points.size(); i1++) {
		chord[i1] = new double*[points.size()];
		vector[i1] = new double*[points.size()];
		sum[i1] = new double[points.size()];
		for (std::size_t i2 = 0; i2 < points.size(); i2++) {
			chord[i1][i2] = new double[3];
			vector[i1][i2] = new double[3];
		}

		for (std::size_t i2 = 0; i2 < i1; i2++) {
			chord[i1][i2][0] = ( points[i1].x + points[points.next(i1)].x -
										points[i2].x - points[points.next(i2)].x ) / 2;
			chord[i1][i2][1] = ( points[i1].y + points[points.next(i1)].y -
										points[i2].y - points[points.next(i2)].y ) / 2;
			chord[i1][i2][2] = ( points[i1].z + points[points.next(i1)].z -
										points[i2].z - points[points.next(i2)].z ) / 2;
			chord_len = sqrt (vector_square (chord[i1][i2]));
			chord[i1][i2][0] /= chord_len;
			chord[i1][i2][1] /= chord_len;
			chord[i1][i2][2] /= chord_len;
			chord[i2][i1][0] = - chord[i1][i2][0];
			chord[i2][i1][1] = - chord[i1][i2][1];
			chord[i2][i1][2] = - chord[i1][i2][2];
			vector_product (tangs[i1], tangs[i2], vector[i1][i2]);
			chord_len *= chord_len;
			vector[i1][i2][0] /= chord_len;
			vector[i1][i2][1] /= chord_len;
			vector[i1][i2][2] /= chord_len;
			vector[i2][i1][0] = - vector[i1][i2][0];
			vector[i2][i1][1] = - vector[i1][i2][1];
			vector[i2][i1][2] = - vector[i1][i2][2];
		}
	}

	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			for (std::size_t i1 = 0; i1 < points.size(); i1++) {
				sum[i1][i1] = 0.0;
				for (std::size_t i2 = points.next(i1); i2 != i1; i2 = points.next(i2)) {
					sum[i1][i2] = sum[i1][points.prev(i2)] + chord[i1][i2][i] * vector[i1][i2][j];
				}
			}

			for (std::size_t i1 = 0; i1 < points.size(); i1++) {
				double tmp = 0.0;

				for (std::size_t i2 = points.next(points.next(i1)); i2 != i1; i2 = points.next(i2)) {
					tmp += sum[points.prev(i2)][points.prev(i1)] - sum[points.prev(i2)][i2] - sum[i2][points.prev(points.prev(i2))] + sum[i2][i1];
					value += tmp * vector[i2][i1][j] * chord[i2][i1][i];
				}
			}
		}
	}

	// Удаляем заранее вычисленные вспомогательные значения.
	for (std::size_t i1 = 0; i1 < points.size(); i1++) {
		for (std::size_t i2 = 0; i2 < points.size(); i2++) {
			delete[] chord[i1][i2];
			delete[] vector[i1][i2];
		}

		delete[] tangs[i1];
		delete[] chord[i1];
		delete[] vector[i1];
		delete[] sum[i1];
	}
	delete[] tangs;
	delete[] chord;
	delete[] vector;
	delete[] sum;

	return value / (64 * M_PI);
}

}}}
