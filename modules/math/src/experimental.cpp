/*
 * Copyright (c) 1995-2021, Mathesis Software <mad@mathesis.fun>
 *
 * Licensed under the Apache License, Version 2.0 the "License";
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <cmath>

#include <KnotWrapper.h>

#include "experimental.h"

namespace KE::ThreeD::Math {

namespace {

double vector_square(const double *v) {
	return v[0] * v[0] + v[1] * v[1] + v[2] * v[2];
}

void vector_product(const double *v1, const double *v2, double *m) {
	m[0] = v1[1] * v2[2] - v1[2] * v2[1];
	m[1] = v1[2] * v2[0] - v1[0] * v2[2];
	m[2] = v1[0] * v2[1] - v1[1] * v2[0];
}

}// namespace

Experimental::Experimental(const KnotWrapper &knot) : Computable(knot, "Experimental") {
}

//double Experimental::compute(const Knot::Snapshot &snapshot) {
//	double value = 0.0;
//
//	std::size_t i1, i2, i3, i4;
//
//	// Вычисляем заранее касательные векторы.
//	double **tangs = new double*[snapshot.size()];
//	for (i1 = 0; i1 < snapshot.size(); i1++)
//	{
//		tangs[i1] = new double[3];
//		tangs[i1][0] = snapshot[snapshot.next(i1)].x - snapshot[i1].x;
//		tangs[i1][1] = snapshot[snapshot.next(i1)].y - snapshot[i1].y;
//		tangs[i1][2] = snapshot[snapshot.next(i1)].z - snapshot[i1].z;
//	}
//
//	// Вычисляем ``гауссовы произведения''.
//	double chord_len;
//	double ***chord = new double**[snapshot.size()];
//	double ***chord2 = new double**[snapshot.size()];
//	double tmp[3], tmp2[3];
//
//	for (i1 = 0; i1 < snapshot.size(); i1++)
//	{
//		chord[i1] = new double*[snapshot.size()];
//		chord2[i1] = new double*[snapshot.size()];
//		for (i2 = 0; i2 < snapshot.size(); i2++)
//		{
//			chord[i1][i2] = new double[3];
//			chord2[i1][i2] = new double[3];
//		}
//
//		for (i2 = 0; i2 < i1; i2++)
//		{
//			chord[i1][i2][0] = ( snapshot[i1].x + snapshot[snapshot.next(i1)].x -
//										snapshot[i2].x - snapshot[snapshot.next(i2)].x ) / 2;
//			chord[i1][i2][1] = ( snapshot[i1].y + snapshot[snapshot.next(i1)].y -
//										snapshot[i2].y - snapshot[snapshot.next(i2)].y ) / 2;
//			chord[i1][i2][2] = ( snapshot[i1].z + snapshot[snapshot.next(i1)].z -
//										snapshot[i2].z - snapshot[snapshot.next(i2)].z ) / 2;
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
//	for (i1 = 0; i1 < snapshot.size(); i1++)
//		for (i2 = i1 + 1; i2 < snapshot.size(); i2++)
//			for (i3 = i2 + 1; i3 < snapshot.size(); i3++)
//				for (i4 = i3 + 1; i4 < snapshot.size(); i4++)
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
//	for (i1 = 0; i1 < snapshot.size(); i1++)
//	{
//		for (i2 = 0; i2 < snapshot.size(); i2++)
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

double Experimental::compute(const Knot::Snapshot &snapshot) {
	double value = 0.0;

	// Вычисляем заранее касательные векторы.
	double **tangs = new double *[snapshot.size()];
	for (std::size_t i1 = 0; i1 < snapshot.size(); i1++) {
		tangs[i1] = new double[3];
		tangs[i1][0] = snapshot[snapshot.next(i1)].x - snapshot[i1].x;
		tangs[i1][1] = snapshot[snapshot.next(i1)].y - snapshot[i1].y;
		tangs[i1][2] = snapshot[snapshot.next(i1)].z - snapshot[i1].z;
	}

	// Вычисляем ``гауссовы произведения''.
	double chord_len;
	double ***chord = new double **[snapshot.size()];
	double ***vector = new double **[snapshot.size()];
	double **sum = new double *[snapshot.size()];

	for (std::size_t i1 = 0; i1 < snapshot.size(); i1++) {
		chord[i1] = new double *[snapshot.size()];
		vector[i1] = new double *[snapshot.size()];
		sum[i1] = new double[snapshot.size()];
		for (std::size_t i2 = 0; i2 < snapshot.size(); i2++) {
			chord[i1][i2] = new double[3];
			vector[i1][i2] = new double[3];
		}

		for (std::size_t i2 = 0; i2 < i1; i2++) {
			chord[i1][i2][0] = (snapshot[i1].x + snapshot[snapshot.next(i1)].x -
													snapshot[i2].x - snapshot[snapshot.next(i2)].x) /
												 2;
			chord[i1][i2][1] = (snapshot[i1].y + snapshot[snapshot.next(i1)].y -
													snapshot[i2].y - snapshot[snapshot.next(i2)].y) /
												 2;
			chord[i1][i2][2] = (snapshot[i1].z + snapshot[snapshot.next(i1)].z -
													snapshot[i2].z - snapshot[snapshot.next(i2)].z) /
												 2;
			chord_len = sqrt(vector_square(chord[i1][i2]));
			chord[i1][i2][0] /= chord_len;
			chord[i1][i2][1] /= chord_len;
			chord[i1][i2][2] /= chord_len;
			chord[i2][i1][0] = -chord[i1][i2][0];
			chord[i2][i1][1] = -chord[i1][i2][1];
			chord[i2][i1][2] = -chord[i1][i2][2];
			vector_product(tangs[i1], tangs[i2], vector[i1][i2]);
			chord_len *= chord_len;
			vector[i1][i2][0] /= chord_len;
			vector[i1][i2][1] /= chord_len;
			vector[i1][i2][2] /= chord_len;
			vector[i2][i1][0] = -vector[i1][i2][0];
			vector[i2][i1][1] = -vector[i1][i2][1];
			vector[i2][i1][2] = -vector[i1][i2][2];
		}
	}

	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			for (std::size_t i1 = 0; i1 < snapshot.size(); i1++) {
				sum[i1][i1] = 0.0;
				for (std::size_t i2 = snapshot.next(i1); i2 != i1; i2 = snapshot.next(i2)) {
					sum[i1][i2] = sum[i1][snapshot.prev(i2)] + chord[i1][i2][i] * vector[i1][i2][j];
				}
			}

			for (std::size_t i1 = 0; i1 < snapshot.size(); i1++) {
				double tmp = 0.0;

				for (std::size_t i2 = snapshot.next(snapshot.next(i1)); i2 != i1; i2 = snapshot.next(i2)) {
					tmp += sum[snapshot.prev(i2)][snapshot.prev(i1)] - sum[snapshot.prev(i2)][i2] - sum[i2][snapshot.prev(snapshot.prev(i2))] + sum[i2][i1];
					value += tmp * vector[i2][i1][j] * chord[i2][i1][i];
				}
			}
		}
	}

	// Удаляем заранее вычисленные вспомогательные значения.
	for (std::size_t i1 = 0; i1 < snapshot.size(); i1++) {
		for (std::size_t i2 = 0; i2 < snapshot.size(); i2++) {
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

}// namespace KE::ThreeD::Math
