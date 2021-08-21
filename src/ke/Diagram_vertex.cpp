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

#include <algorithm>

#include "Diagram.h"

namespace KE::TwoD {

std::shared_ptr<Diagram::Vertex> Diagram::addVertex(int x, int y) {
	if (this->isClosed()) {
		return nullptr;
	}

	std::shared_ptr<Vertex> new_vertex(new Vertex(x, y));
	if (this->_vertices.empty()) {
		this->_vertices.push_back(new_vertex);
	} else {
		std::shared_ptr<Vertex> end = this->_vertices.back();
		this->_vertices.push_back(new_vertex);
		const Edge new_edge(end, new_vertex);
		for (const Edge &e : this->edges()) {
			if (e.intersects(new_edge)) {
				this->addCrossing(new_edge, e);
				e.orderCrossings(e.start->crossings);
			}
		}
	}
	return new_vertex;
}

std::shared_ptr<Diagram::Vertex> Diagram::addVertex(const Edge &edge, int x, int y) {
	std::shared_ptr<Vertex> new_vertex(new Vertex(x, y));
	auto iter = std::find(this->_vertices.begin(), this->_vertices.end(), edge.end);
	this->_vertices.insert(iter, new_vertex);

	const Edge new1(edge.start, new_vertex);
	const Edge new2(new_vertex, edge.end);

	for (const Edge &e : this->edges()) {
		auto removed_crossing = this->getCrossing(edge, e);
		this->removeCrossing(edge, e);

		for (const Edge &new_edge : {new1, new2}) {
			if (e.intersects(new_edge)) {
				if (removed_crossing && removed_crossing->up == e) {
					this->addCrossing(e, new_edge);
				} else {
					this->addCrossing(new_edge, e);
				}
			}
		}
	}

	this->order();
	return new_vertex;
}

void Diagram::removeVertex(const std::shared_ptr<Vertex> &vertex) {
	std::shared_ptr<const Edge> removed1;
	std::shared_ptr<const Edge> removed2;
	for (const Edge &edge : this->edges()) {
		if (edge.end == vertex) {
			removed1 = std::make_shared<const Edge>(edge);
		} else if (edge.start == vertex) {
			removed2 = std::make_shared<const Edge>(edge);
		}
	}
	std::shared_ptr<const Edge> merged;
	if (removed1 && removed2) {
		merged = std::make_shared<const Edge>(removed1->start, removed2->end);
	}

	this->_vertices.remove(vertex);

	for (const Edge &edge : this->edges()) {
		std::shared_ptr<Crossing> removed_crossing1;
		if (removed1) {
			removed_crossing1 = this->getCrossing(*removed1, edge);
			this->removeCrossing(*removed1, edge);
		}
		std::shared_ptr<Crossing> removed_crossing2;
		if (removed2) {
			removed_crossing2 = this->getCrossing(*removed2, edge);
			this->removeCrossing(*removed2, edge);
		}

		if (!merged || !edge.intersects(*merged)) {
			continue;
		}
		if ((removed_crossing1 && removed_crossing1->up == edge) ||
				(removed_crossing2 && removed_crossing2->up == edge)) {
			this->addCrossing(edge, *merged);
		} else {
			this->addCrossing(*merged, edge);
		}
	}
}

void Diagram::moveVertex(const std::shared_ptr<Vertex> &vertex, int x, int y) {
	vertex->moveTo(x, y);

	std::shared_ptr<const Edge> changed1;
	std::shared_ptr<const Edge> changed2;
	for (const Edge &edge : this->edges()) {
		if (edge.end == vertex) {
			changed1 = std::make_shared<const Edge>(edge);
		} else if (edge.start == vertex) {
			changed2 = std::make_shared<const Edge>(edge);
		}
	}

	for (const Edge &edge : this->edges()) {
		auto changed_crossing1 = changed1 ? this->getCrossing(*changed1, edge) : nullptr;
		auto changed_crossing2 = changed2 ? this->getCrossing(*changed2, edge) : nullptr;

		if (changed1) {
			if (edge.intersects(*changed1)) {
				if (!changed_crossing1) {
					if (changed_crossing2 && changed_crossing2->up == edge) {
						this->addCrossing(edge, *changed1);
					} else {
						this->addCrossing(*changed1, edge);
					}
				}
			} else {
				this->removeCrossing(edge, *changed1);
			}
		}
		if (changed2) {
			if (edge.intersects(*changed2)) {
				if (!changed_crossing2) {
					if (changed_crossing1 && changed_crossing1->up == edge) {
						this->addCrossing(edge, *changed2);
					} else {
						this->addCrossing(*changed2, edge);
					}
				}
			} else {
				this->removeCrossing(edge, *changed2);
			}
		}
	}

	this->order();
}

void Diagram::close() {
	if (this->_isClosed) {
		return;
	}

	const auto edges = this->edges();
	if (edges.size() < 2) {
		return;
	}

	const Edge new_edge(edges.back().end, edges.front().start);
	for (const Edge &edge : edges) {
		if (new_edge.intersects(edge)) {
			this->addCrossing(new_edge, edge);
		}
	}
	this->_isClosed = true;
}

void Diagram::removeEdge(const Edge &edge) {
	const auto edges = this->edges();
	if (edges.empty()) {
		return;
	}

	if (this->isClosed()) {
		const auto loc = std::find(edges.begin(), edges.end(), edge);
		if (loc == edges.end()) {
			return;
		}
		std::list<std::shared_ptr<Vertex>> new_list;
		for (auto it = loc; it != edges.end(); ++it) {
			new_list.push_back(it->end);
		}
		for (auto it = edges.begin(); it != loc; ++it) {
			new_list.push_back(it->end);
		}
		// TODO: remove crossings
		new_list.back()->crossings.clear();
		for (const auto &vertex : new_list) {
			vertex->crossings.remove_if([edge](const Crossing &crossing) { return crossing.up == edge; });
		}
		this->_vertices.swap(new_list);
		this->_isClosed = false;
	} else {
		if (edge == edges.front()) {
			this->removeVertex(edge.start);
			if (edges.size() == 1) {
				this->removeVertex(edge.end);
			}
		} else if (edge == edges.back()) {
			this->removeVertex(edge.end);
		}
	}
}

}
