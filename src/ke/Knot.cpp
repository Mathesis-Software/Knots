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
#include <map>

#include "Diagram.h"
#include "Knot.h"

namespace KE::ThreeD {

Knot::Knot(const std::vector<Point> &points, const std::string &caption) : caption(caption), generation(1), lockCount(0) {
	this->_points = points;

	double min = this->_points.front().distanceTo(this->_points.back());
	double total = min;
	for (std::size_t i = 1; i < this->_points.size(); ++i) {
		const double dist = this->_points[i - 1].distanceTo(this->_points[i]);
		min = std::min(min, dist);
		total += dist;
	}
	this->normalize(std::max(5 * this->_points.size(), 3 * (std::size_t)std::round(total / min)));
	this->center();
}

void Knot::normalize(std::size_t newNumberOfPoints) {
	std::lock_guard<std::mutex> writeMethodGuard(this->writeMethodMutex);
	const auto snapshot = this->snapshot();
	auto normalized = this->normalizedPoints(snapshot, newNumberOfPoints);

	counting_lock guard(*this);
	this->_points.swap(normalized);
}

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

std::vector<Point> Knot::pointsFromDiagram(const TwoD::Diagram &diagram, std::size_t width, std::size_t height) {
	std::vector<Point> points;

	const auto edges = diagram.edges();
	auto all_crossings = diagram.allCrossings();

	for (const auto &edge : edges) {
		const auto coords = edge.start->coords();
		points.push_back(Point(
			2.4 * coords.x / width - 1.2,
			1.2 - 2.4 * coords.y / height,
			0
		));
		for (const auto &crs : all_crossings[edge]) {
			std::shared_ptr<TwoD::FloatPoint> current = crs.coords();
			if (!current) {
				continue;
			}
			points.push_back(Point(
				2.4 * current->x / width - 1.2,
				1.2 - 2.4 * current->y / height,
				crs.up == edge ? 0.2 : -0.2
			));
		}
	}

	return points;
}

}
