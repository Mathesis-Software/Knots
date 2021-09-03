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

#include <map>
#include <set>

#include "DiagramProperty.h"
#include "Polynomial.h"
#include "SquareMatrix.h"
#include "../ke/Diagram.h"

namespace KE::TwoD::Math {

namespace {

struct CrossingEx {
	const Diagram::Crossing cro;
	const bool over;

	CrossingEx(const Diagram::Crossing &cro, bool over) : cro(cro), over(over) {}
	bool clockwise() const {
		const Diagram::Edge &edge0 = this->over ? this->cro.up : this->cro.down;
		const Diagram::Edge &edge1 = this->over ? this->cro.down : this->cro.up;
		return edge0.dx() * edge1.dy() - edge1.dx() * edge0.dy() < 0;
	}
	CrossingEx inversion() const {
		return CrossingEx(this->cro, !this->over);
	}
	bool operator == (const CrossingEx &ex) const {
		return this->cro == ex.cro && this->over == ex.over;
	}
	bool operator != (const CrossingEx &ex) const {
		return !(*this == ex);
	}
	bool operator < (const CrossingEx &ex) const {
		return this->cro < ex.cro || (this->cro == ex.cro && this->over < ex.over);
	}
};

struct Face {
	enum Role {
		leftBefore,
		rightBefore,
		leftAfter,
		rightAfter
	};

	const bool direction;
	std::list<CrossingEx> crossings;
	std::map<Diagram::Crossing,Role> matrixRow;

	Face(bool direction, const CrossingEx &vertex) : direction(direction) {
		this->add(vertex);
	}
	bool add(CrossingEx vertex) {
		if (this->direction) {
			if (this->crossings.size() > 0 && vertex == this->crossings.front()) {
				return false;
			}
			this->crossings.push_back(vertex);
		} else {
			if (this->crossings.size() > 0 && vertex == this->crossings.back().inversion()) {
				return false;
			}
			this->crossings.push_front(vertex.inversion());
		}
		return true;
	}
	CrossingEx lastKey() const {
		return this->direction ? this->crossings.back().inversion() : this->crossings.front();
	}
	std::vector<std::pair<CrossingEx,CrossingEx>> edges() const {
		std::vector<std::pair<CrossingEx,CrossingEx>> pairs;
		auto prev = this->crossings.end();
		for (auto iter = this->crossings.begin(); iter != this->crossings.end(); ++iter) {
			if (prev != this->crossings.end()) {
				pairs.push_back(std::make_pair(*prev, *iter));
			}
			prev = iter;
		}
		pairs.push_back(std::make_pair(crossings.back(), crossings.front()));
		return pairs;
	}
	void computeMatrixRow(std::function<bool(const std::pair<CrossingEx,CrossingEx>&)> isPositive) {
		for (const auto &edge : this->edges()) {
			if (!edge.first.over) {
				this->matrixRow[edge.first.cro] = isPositive(edge) ? leftAfter : rightBefore;
			}
			if (edge.second.over) {
				this->matrixRow[edge.second.cro] = isPositive(edge) ? leftBefore : rightAfter;
			}
		}
	}
};

std::vector<Face> collectFaces(const Diagram &diagram) {
	const auto edges = diagram.edges();
	auto edge2Crossings = diagram.allCrossings();

	std::vector<CrossingEx> all;
	for (const auto &edge : edges) {
		for (const auto &cro : edge2Crossings[edge]) {
			all.push_back(CrossingEx(cro, cro.up == edge));
		}
	}
	std::map<CrossingEx,CrossingEx> next;
	std::map<CrossingEx,CrossingEx> prev;
	for (std::size_t i = 0; i < all.size() - 1; i += 1) {
		next.insert(std::make_pair(all[i], all[i + 1]));
		prev.insert(std::make_pair(all[i + 1], all[i]));
	}
	next.insert(std::make_pair(all.back(), all.front()));
	prev.insert(std::make_pair(all.front(), all.back()));

	std::vector<Face> faces;
	std::set<std::pair<CrossingEx,CrossingEx>> used;
	for (const auto &ex : all) {
		for (bool direction : {false, true}) {
			Face face(direction, ex);
			auto cw = direction;
			while (true) {
				const auto key = face.lastKey();
				if (key.clockwise() == cw) {
					if (!face.add(next.find(key)->second)) {
						break;
					}
					cw = direction;
				} else {
					if (!face.add(prev.find(key)->second)) {
						break;
					}
					cw = !direction;
				}
			}

			std::size_t used_count = 0;
			const auto edges = face.edges();
			for (const auto &edge : face.edges()) {
				if (used.find(edge) != used.end()) {
					used_count += 1;
				}
			}
			if (used_count == face.crossings.size()) {
				continue;
			}

			used.insert(edges.begin(), edges.end());

			face.computeMatrixRow([&next](const std::pair<CrossingEx,CrossingEx> &edge) {
				return next.find(edge.first.inversion())->second == edge.second;
			});
			faces.push_back(face);
		}
	}

	return faces;
}

Polynomial poly(Face::Role role) {
	switch (role) {
		default:
			return Polynomial::ZERO;
		case Face::leftBefore:
			return Polynomial::MINUS_T;
		case Face::rightBefore:
			return Polynomial::ONE;
		case Face::leftAfter:
			return Polynomial::T;
		case Face::rightAfter:
			return Polynomial::MINUS_ONE;
	}
}

}

bool AlexanderPolynomial::isApplicable(const Diagram &diagram) const {
	return diagram.isClosed();
}

Polynomial AlexanderPolynomial::value(const Diagram &diagram) const {
	if (!diagram.hasCrossings()) {
		return Polynomial::ONE;
	}

	const auto faces = collectFaces(diagram);

	std::map<Diagram::Crossing,int> indices;

	auto edge0 = faces[0].edges()[0];
	auto edge1 = std::make_pair(edge0.second.inversion(), edge0.first.inversion());
	std::vector<std::vector<Polynomial>> rows;
	std::size_t size = faces.size() - 2;

	for (const auto &face : faces) {
		const auto edges = face.edges();
		if (std::find(edges.begin(), edges.end(), edge0) != edges.end() || std::find(edges.begin(), edges.end(), edge1) != edges.end()) {
			continue;
		}
		std::vector<Polynomial> row(size, Polynomial::ZERO);
		for (const auto &[cro, role] : face.matrixRow) {
			if (indices.find(cro) == indices.end()) {
				indices.insert(std::make_pair(cro, indices.size()));
			}
			row[indices[cro]] = poly(role);
		}
		rows.push_back(row);
	}

	ConstMatrix matrix(rows);
	return matrix.determinant().reduced();
}

}
