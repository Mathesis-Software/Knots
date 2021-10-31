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

#include "computables.h"

namespace KE::ThreeD::Math {

VassilievInvariant::VassilievInvariant(const KnotWrapper &knot, int order) : Computable(knot, "Order " + std::to_string(order) + " Vassiliev invariant"),
																																						 order(order) {
}

double VassilievInvariant::compute(const Knot::Snapshot &snapshot) {
	double value = 0.0;

	// tangent vectors
	std::vector<Vector> tangents;
	for (std::size_t i = 0; i < snapshot.size(); ++i) {
		tangents.push_back(Vector(snapshot[i], snapshot[snapshot.next(i)]));
	}

	// Вычисляем ``гауссовы произведения''.
	std::vector<std::vector<double>> gauss(snapshot.size());
	for (auto &v : gauss) {
		v.insert(v.end(), snapshot.size(), 0.0);
	}
	for (std::size_t i = 0; i < snapshot.size(); ++i) {
		for (std::size_t j = 0; j < i; j++) {
			const Vector chord(
							(snapshot[i].x + snapshot[snapshot.next(i)].x - snapshot[j].x - snapshot[snapshot.next(j)].x) / 2,
							(snapshot[i].y + snapshot[snapshot.next(i)].y - snapshot[j].y - snapshot[snapshot.next(j)].y) / 2,
							(snapshot[i].z + snapshot[snapshot.next(i)].z - snapshot[j].z - snapshot[snapshot.next(j)].z) / 2);
			double chord_len = chord.length();
			gauss[i][j] = tangents[i].scalar_product(tangents[j].vector_product(chord)) / (chord_len * chord_len * chord_len);
			gauss[j][i] = gauss[i][j];
		}
	}

	// Вычисляем суммы ``гауссовых произведений''.

	// В gauss_sum[i1][i2] находится сумма ``гауссовых произведений''
	// для всех хорд с началом в i1 и концом от snapshot.next(i1) до i2.
	std::vector<std::vector<double>> gauss_sum(snapshot.size());
	for (auto &v : gauss_sum) {
		v.insert(v.end(), snapshot.size(), 0.0);
	}
	for (std::size_t i = 0; i < snapshot.size(); i++) {
		for (std::size_t j = snapshot.next(i); j != i; j = snapshot.next(j)) {
			gauss_sum[i][j] = gauss_sum[i][snapshot.prev(j)] + gauss[i][j];
		}
	}

	for (std::size_t i = 0; i < snapshot.size(); i++) {
		double tmp = 0.0;
		for (std::size_t j = snapshot.next(snapshot.next(i)); j != i; j = snapshot.next(j)) {
			tmp += gauss_sum[snapshot.prev(j)][snapshot.prev(i)] -
						 gauss_sum[j][snapshot.prev(snapshot.prev(j))] + gauss_sum[j][i];
			double tmp2 = gauss[i][j];
			for (int o = 1; o < order; o++) {
				tmp2 *= tmp / 16;
			}
			value += tmp2;
		}
	}

	return value / (4 * M_PI * M_PI);
}

}// namespace KE::ThreeD::Math
