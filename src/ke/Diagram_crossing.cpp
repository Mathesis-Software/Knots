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

#include <functional>

#include "Diagram.h"

namespace KE::TwoD {

std::shared_ptr<FloatPoint> Diagram::Crossing::coords() const {
	const int d0 = this->up.dy() * this->down.dx() - this->up.dx() * this->down.dy();

	if (d0 == 0) {
		return nullptr;
	}

	const int d1 =
			(this->down.start->y() - this->up.start->y()) * this->down.dx()
		-	(this->down.start->x() - this->up.start->x()) * this->down.dy();

	return std::make_shared<FloatPoint>(
		this->up.start->x() + 1.0 * this->up.dx() * d1 / d0,
		this->up.start->y() + 1.0 * this->up.dy() * d1 / d0
	);
}

bool Diagram::hasCrossings() const {
	for (const auto &vertex : this->vertices()) {
		if (!vertex->crossings.empty()) {
			return true;
		}
	}

	return false;
}

std::shared_ptr<Diagram::Crossing> Diagram::addCrossing(const Edge &up, const Edge &down) {
	this->removeCrossing(up, down);

	if (!up.intersects(down)) {
		return nullptr;
	}

	std::shared_ptr<Crossing> new_crossing = std::make_shared<Crossing>(up, down);
	down.start->crossings.push_back(*new_crossing);
	down.orderCrossings(down.start->crossings);
	return new_crossing;
}

std::shared_ptr<Diagram::Crossing> Diagram::flipCrossing(Crossing &crs) {
	return this->addCrossing(crs.down, crs.up);
}

void Diagram::removeCrossing(const Edge &edge1, const Edge &edge2) {
	edge1.start->crossings.remove(Crossing(edge2, edge1));
	edge2.start->crossings.remove(Crossing(edge1, edge2));
}

std::shared_ptr<Diagram::Crossing> Diagram::getCrossing(const Edge &edge1, const Edge &edge2) {
	for (const auto &crs : this->crossings(edge1)) {
		if (crs.up == edge2) {
			return std::make_shared<Crossing>(crs);
		}
	}
	for (const auto &crs : this->crossings(edge2)) {
		if (crs.up == edge1) {
			return std::make_shared<Crossing>(crs);
		}
	}

	return nullptr;
}

void Diagram::Edge::orderCrossings(std::list<Crossing> &crossings) const {
	std::function<bool(const Crossing&,const Crossing&)> comparator;
	if (abs(this->dx()) > abs(this->dy())) {
		if (this->dx() > 0) {
			comparator = [](const Crossing &c0, const Crossing &c1) { return c0.coords()->x < c1.coords()->x; };
		} else {
			comparator = [](const Crossing &c0, const Crossing &c1) { return c0.coords()->x > c1.coords()->x; };
		}
	} else {
		if (this->dy() > 0) {
			comparator = [](const Crossing &c0, const Crossing &c1) { return c0.coords()->y < c1.coords()->y; };
		} else {
			comparator = [](const Crossing &c0, const Crossing &c1) { return c0.coords()->y > c1.coords()->y; };
		}
	}
	crossings.sort(comparator);
}

}
