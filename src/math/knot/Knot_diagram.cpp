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

#include <cmath>
#include <map>

#include "Knot.h"
#include "../diagram/Diagram.h"

namespace KE { namespace ThreeD {

Knot::Knot(const TwoD::Diagram &diagram, std::size_t width, std::size_t height) : caption(diagram.caption + " (from diagram)"), generation(1), lockCount(0) {
	const auto edges = diagram.edges();
	std::map<std::shared_ptr<TwoD::Diagram::Vertex>,std::list<TwoD::Diagram::Crossing>> all_crossings;
	for (const auto &edge : edges) {
		const auto crossings = diagram.crossings(edge);
		auto &list = all_crossings[edge.start];
		list.insert(list.end(), crossings.begin(), crossings.end());
		for (const auto &crs : crossings) {
			all_crossings[crs.up.start].push_back(crs);
		}
	}

	for (const auto &edge : edges) {
		const auto coords = edge.start->coords();
		this->_points.push_back(Point(
			2.4 * coords.x / width - 1.2,
			1.2 - 2.4 * coords.y / height,
			0
		));
		auto &crossings = all_crossings[edge.start];
		edge.orderCrossings(crossings);
		for (const auto &crs : crossings) {
			std::shared_ptr<TwoD::FloatPoint> current = crs.coords();
			if (!current) {
				continue;
			}
			this->_points.push_back(Point(
				2.4 * current->x / width - 1.2,
				1.2 - 2.4 * current->y / height,
				crs.up == edge ? 0.2 : -0.2
			));
		}
	}

	double min = this->_points.front().distanceTo(this->_points.back());
	double total = min;
	for (std::size_t i = 1; i < this->_points.size(); ++i) {
		const double dist = this->_points[i - 1].distanceTo(this->_points[i]);
		min = std::min(min, dist);
		total += dist;
	}
	normalize(std::max(5 * this->_points.size(), 3 * (std::size_t)std::round(total / min)));
	center();
}

}}
