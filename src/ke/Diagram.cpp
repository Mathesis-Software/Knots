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

#include <algorithm>
#include <limits>
#include <stdexcept>

#include "Diagram.h"

namespace KE::TwoD {

Diagram::Diagram() : caption("New Diagram"), _isClosed(false) {
}

void Diagram::clear() {
	this->_vertices.clear();
	this->_isClosed = false;
}

void Diagram::order() {
	for (Edge &edge : this->edges()) {
		edge.orderCrossings(edge.start->crossings);
	}
}

void Diagram::shift(int dx, int dy) {
	for (auto vertex : this->vertices()) {
		vertex->move(dx, dy);
	}
}

std::list<Diagram::Edge> Diagram::edges() const {
	std::list<Edge> edges;

	std::shared_ptr<Vertex> previous = nullptr;
	for (auto vertex : this->vertices()) {
		if (previous) {
			edges.push_back(Edge(previous, vertex));
		}
		previous = vertex;
	}
	if (this->_isClosed && this->vertices().size() >= 2) {
		edges.push_back(Edge(this->vertices().back(), this->vertices().front()));
	}

	return edges;
}

std::shared_ptr<Diagram::Vertex> Diagram::findVertex(const FloatPoint &pt, float maxDistance) const {
	float best = std::numeric_limits<float>::max();
	std::shared_ptr<Vertex> found;

	for (auto vertex : this->vertices()) {
		const float distance = pt.distance(vertex->coords());
		if (distance < best && distance <= maxDistance) {
			best = distance;
			found = vertex;
		}
	}

	return found;
}

std::shared_ptr<Diagram::Crossing> Diagram::findCrossing(const FloatPoint &pt, float maxDistance) const {
	float best = std::numeric_limits<float>::max();
	std::shared_ptr<Crossing> found;

	for (const auto &edge : this->edges()) {
		for (const auto &crs : this->underCrossings(edge)) {
			const auto coords = crs.coords();
			if (!coords) {
				continue;
			}
			const float distance = pt.distance(*coords);
			if (distance < best && distance <= maxDistance) {
				best = distance;
				found = std::make_shared<Crossing>(crs);
			}
		}
	}

	return found;
}

std::shared_ptr<Diagram::Edge> Diagram::findEdge(const FloatPoint &pt, float maxDistance) const {
	float best = std::numeric_limits<float>::max();
	std::shared_ptr<Edge> found;

	for (const Edge &edge : this->edges()) {
		const float dx = edge.dx();
		const float dy = edge.dy();

		if (dx == 0 && dy == 0) {
			// the edge has zero length
			continue;
		}

		const auto start = edge.start->coords();
		const auto end = edge.end->coords();
		if ((pt.x - start.x) * dx + (pt.y - start.y) * dy < 0 ||
				(pt.x - end.x) * dx + (pt.y - end.y) * dy > 0) {
			// pt is outside of the perpendicular strip built on the edge segment
			continue;
		}

		const float distance = fabs((pt.x - start.x) * dy - (pt.y - start.y) * dx) / hypotf(dx, dy);
		if (distance < best && distance <= maxDistance) {
			best = distance;
			found = std::make_shared<Edge>(edge);
		}
	}

	return found;
}

namespace {

bool orientation(const std::shared_ptr<Diagram::Vertex> &v0, const std::shared_ptr<Diagram::Vertex> &v1, const std::shared_ptr<Diagram::Vertex> &v2) {
	const auto pt0 = v0->coords();
	const auto pt1 = v1->coords();
	const auto pt2 = v2->coords();
	const float area = pt0.x * (pt1.y - pt2.y) + pt1.x * (pt2.y - pt0.y) + pt2.x * (pt0.y - pt1.y);
	if (area < 0) {
		return false;
	} else if (area > 0) {
		return true;
	} else {
		int count = 0;
		count += v0->index < v1->index ? 1 : 0;
		count += v0->index < v2->index ? 1 : 0;
		count += v1->index < v2->index ? 1 : 0;
		return count % 2 == 0;
	}
}

}

bool Diagram::Edge::intersects(const Diagram::Edge &edge) const {
	if (this->start == edge.start ||
			this->start == edge.end ||
			this->end == edge.start ||
			this->end == edge.end) {
		return false;
	}
	const bool ori = orientation(this->start, edge.start, this->end);
	return
		ori == orientation(edge.start, this->end, edge.end) &&
		ori == orientation(this->end, edge.end, this->start) &&
		ori == orientation(edge.end, this->start, edge.start);
}

std::map<Diagram::Edge,std::list<Diagram::Crossing>> Diagram::allCrossings() const {
	const auto edges = this->edges();
	std::map<Diagram::Edge,std::list<Diagram::Crossing>> map;
	for (const auto &edge : edges) {
		const auto crossings = this->underCrossings(edge);
		auto &list = map[edge];
		list.insert(list.end(), crossings.begin(), crossings.end());
		for (const auto &crs : crossings) {
			map[crs.up].push_back(crs);
		}
	}
	for (const auto &edge : edges) {
		edge.orderCrossings(map[edge]);
	}
	return map;
}

}
