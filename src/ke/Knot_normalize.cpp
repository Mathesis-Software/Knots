/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at

 *   http://www.apache.org/licenses/LICENSE-2.0

 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 *
 * Author: Nikolay Pultsin <geometer@geometer.name>
 */

#include "Knot.h"

namespace KE::ThreeD {

std::vector<Point> Knot::normalizedPoints(const Snapshot &snapshot, std::size_t newNumberOfPoints) {
	const auto &edgeLengths = snapshot.edgeLengths();
	const double totalLength = snapshot.knotLength();
	const double newEdgeLength = totalLength / newNumberOfPoints;

	std::vector<Point> newPoints;
	newPoints.reserve(newNumberOfPoints);

	std::size_t v = 0;
	double llen = edgeLengths[0];
	double rest = llen;

	for (std::size_t i = 0; i < newNumberOfPoints; i++) {
		const auto nextV = v == snapshot.size() - 1 ? 0 : v + 1;

		const Vector delta(snapshot[v], snapshot[nextV]);
		Point pt(snapshot[v]);
		pt.move(delta, 1 - rest / llen);
		newPoints.push_back(pt);

		rest -= newEdgeLength;
		while (rest < 0) {
			v = v == snapshot.size() - 1 ? 0 : v + 1;
			llen = edgeLengths[v];
			rest += llen;
		}
	}

	return newPoints;
}

void Knot::normalize(std::size_t newNumberOfPoints) {
	std::lock_guard<std::mutex> writeMethodGuard(this->writeMethodMutex);
	const auto snapshot = this->snapshot();
	auto normalized = this->normalizedPoints(snapshot, newNumberOfPoints);

	counting_lock guard(*this);
	this->_points.swap(normalized);
}

}
