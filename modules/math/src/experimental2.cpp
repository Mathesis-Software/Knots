/*
 * Copyright (c) 1995-2021, Nikolay Pultsin <geometer@geometer.name>
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

double Experimental2::compute(const Knot::Snapshot &snapshot) {
	double value = 0.0;

	std::size_t i1, i2;
	int o;

	// Вычисляем заранее касательные векторы.
	double **tangs = new double*[snapshot.size()];
	for (i1 = 0; i1 < snapshot.size(); i1++) {
		tangs[i1] = new double[3];
		tangs[i1][0] = snapshot[snapshot.next(i1)].x - snapshot[i1].x;
		tangs[i1][1] = snapshot[snapshot.next(i1)].y - snapshot[i1].y;
		tangs[i1][2] = snapshot[snapshot.next(i1)].z - snapshot[i1].z;
	}

	// Вычисляем ``гауссовы произведения''.
	double chord[3], chord_len;
	double **gauss = new double*[snapshot.size()];

	for (i1 = 0; i1 < snapshot.size(); i1++) {
		gauss[i1] = new double[snapshot.size()];
		gauss[i1][i1] = 0.0;
		for (i2 = 0; i2 < i1; i2++) {
			chord[0] = ( snapshot[i1].x + snapshot[snapshot.next(i1)].x -
										snapshot[i2].x - snapshot[snapshot.next(i2)].x ) / 2;
			chord[1] = ( snapshot[i1].y + snapshot[snapshot.next(i1)].y -
										snapshot[i2].y - snapshot[snapshot.next(i2)].y ) / 2;
			chord[2] = ( snapshot[i1].z + snapshot[snapshot.next(i1)].z -
										snapshot[i2].z - snapshot[snapshot.next(i2)].z ) / 2;
			chord_len = sqrt (vector_square (chord));
			gauss[i1][i2] = det (tangs[i1], tangs[i2], chord) /
												(chord_len * chord_len * chord_len);
			gauss[i2][i1] = gauss[i1][i2];
		}
	}

	// Вычисляем суммы ``гауссовых произведений''.

	// В gauss_sum[i1][i2] находится сумма ``гауссовых произведений''
	// для всех хорд с началом в i1 и концом от snapshot.next(i1) до i2.
	double **gauss_sum = new double*[snapshot.size()];

	for (i1 = 0; i1 < snapshot.size(); i1++) {
		gauss_sum[i1] = new double[snapshot.size()];
		gauss_sum[i1][i1] = 0.0;
		for (i2 = snapshot.next(i1); i2 != i1; i2 = snapshot.next(i2))
			gauss_sum[i1][i2] = gauss_sum[i1][snapshot.prev(i2)] + gauss[i1][i2];
	}

	double tmp, tmp2;
	for (i1 = 0; i1 < snapshot.size(); i1++) {
		tmp = 0.0;
		for (i2 = snapshot.next(snapshot.next(i1)); i2 != i1; i2 = snapshot.next(i2)) {
			tmp += gauss_sum[snapshot.prev(i2)][snapshot.prev(i1)] -
						 gauss_sum[i2][snapshot.prev(snapshot.prev(i2))] + gauss_sum[i2][i1];
			tmp2 = gauss[i1][i2];
			for (o = 1; o < order; o++)
				tmp2 *= tmp / 16;
			value += tmp2;
		}
	}

	// Удаляем заранее вычисленные вспомогательные значения.
	for (i1 = 0; i1 < snapshot.size(); i1++) {
		delete[] tangs[i1];
		delete[] gauss[i1];
		delete[] gauss_sum[i1];
	}
	delete[] tangs;
	delete[] gauss;
	delete[] gauss_sum;

	return value / (4 * M_PI * M_PI);
}

}
