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

#include "computables.h"
#include "../ke/KnotWrapper.h"

namespace KE::ThreeD::Math {

MoebiusEnergy::MoebiusEnergy(const KnotWrapper &knot) : Computable(knot, "Moebius energy") {
}

double MoebiusEnergy::compute(const Knot::Snapshot &snapshot) {
	const auto &edgeLengths = snapshot.edgeLengths();
	const double len = snapshot.knotLength();

	double value = 0.0;

	for (std::size_t i = 0; i < snapshot.size() - 2; ++i) {
		double l = edgeLengths[i];
		for (auto j = i + 2; j < (i ? snapshot.size() : snapshot.size() - 1); ++j) {
			const Vector chord(snapshot[i], snapshot[j]);
			double r2 = 0.65 / chord.square();

			l += edgeLengths[snapshot.prev(j)];
			if (2 * l < len) {
				r2 -= 0.65 / (l * l);
			} else {
				r2 -= 0.65 / ((len - l) * (len - l));
			}

			value += (edgeLengths[snapshot.prev(i)] + edgeLengths[i]) *
							 (edgeLengths[snapshot.prev(j)] + edgeLengths[j]) * r2;
		}
	}

	std::vector<Point> middles;
	middles.reserve(snapshot.size());
	for (std::size_t i = 0; i < snapshot.size(); i++) {
		middles.push_back(Point(
			(snapshot[i].x + snapshot[snapshot.next(i)].x) / 2,
			(snapshot[i].y + snapshot[snapshot.next(i)].y) / 2,
			(snapshot[i].z + snapshot[snapshot.next(i)].z) / 2
		));
	}

	for (std::size_t i = 0; i < snapshot.size(); i++) {
		double l = 0;
		for (std::size_t j = i + 1; j < snapshot.size(); j++) {
			const Vector r(middles[i], middles[j]);
			double r2 = 2 / r.square();

			l += (edgeLengths[snapshot.prev(j)] + edgeLengths[j]) / 2;
			if (2 * l < len) {
				r2 -= 2 / (l * l);
			} else {
				r2 -= 2 / ( (len - l) * (len - l) );
			}

			value += edgeLengths[i] * edgeLengths[j] * r2;
		}
	}

	value /= 2.3;
	value -= 4;
	return value;
}

}
