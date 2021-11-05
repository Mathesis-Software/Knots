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

AverageExtremumNumber::AverageExtremumNumber(const KnotWrapper &knot) : Computable(knot, "Average extremum number") {
}

double AverageExtremumNumber::compute(const Knot::Snapshot &snapshot) {
	const auto &edgeLengths = snapshot.edgeLengths();

	std::vector<Vector> edges;
	edges.reserve(snapshot.size());
	for (std::size_t i = 0; i < snapshot.size() - 1; ++i) {
		edges.push_back(Vector(snapshot[i], snapshot[i + 1]));
	}
	edges.push_back(Vector(snapshot[snapshot.size() - 1], snapshot[0]));

	double value = 0;

	for (std::size_t i = 0; i < snapshot.size(); ++i) {
		const auto &forward = edges[i];
		const auto &back = edges[snapshot.prev(i)];
		double angle_cos = -forward.scalar_product(back) / (edgeLengths[snapshot.prev(i)] * edgeLengths[i]);

		if (angle_cos < -1) {
			value -= M_PI;
		} else if (angle_cos < 1) {
			value -= acos(angle_cos);
		}
	}

	return value / M_PI + snapshot.size();
}

}
