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

typedef std::pair<CrossingEx,CrossingEx> Bridge;
struct Face {
	enum Role {
		leftBefore,
		rightBefore,
		leftAfter,
		rightAfter
	};

	std::list<std::pair<CrossingEx,Role>> matrixRow;

	Bridge firstBridge() const {
		return std::make_pair(this->matrixRow.back().first.inversion(), this->matrixRow.front().first);
	}

	bool matches(const Bridge &bridge) const {
		bool flag = this->matrixRow.back().first.inversion() == bridge.first;
		for (const auto &pair : this->matrixRow) {
			if (flag && pair.first == bridge.second) {
				return true;
			}
			flag = pair.first.inversion() == bridge.first;
		}
		return false;
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
		next.emplace(all[i], all[i + 1]);
		prev.emplace(all[i + 1], all[i]);
	}
	next.emplace(all.back(), all.front());
	prev.emplace(all.front(), all.back());

	std::map<Bridge,std::pair<CrossingEx,Face::Role>> angles;
	for (const auto &crs : all) {
		Bridge forward(prev.at(crs), crs);
		Bridge back(next.at(crs), crs);
		angles.emplace(forward, std::make_pair(
			crs.clockwise() ? next.at(crs.inversion()) : prev.at(crs.inversion()),
			crs.over ? Face::rightBefore : (crs.clockwise() ? Face::rightAfter : Face::leftBefore)
		));
		angles.emplace(back, std::make_pair(
			crs.clockwise() ? prev.at(crs.inversion()): next.at(crs.inversion()),
			crs.over ? Face::leftAfter : (crs.clockwise() ? Face::leftBefore : Face::rightAfter)
		));
	}

	std::vector<Face> faces;
	while (!angles.empty()) {
		Face face;
		std::list<Bridge> stack;
		const auto bridge = angles.begin()->first;
		stack.push_back(bridge);
		while (true) {
			const auto &last = stack.back();
			const auto &angle = angles.at(last);
			face.matrixRow.push_back(std::make_pair(last.second, angle.second));
			Bridge b(last.second.inversion(), angle.first);
			angles.extract(last);
			if (b == stack.front()) {
				break;
			}
			stack.push_back(b);
		}
		faces.push_back(face);
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

	auto edge0 = faces[0].firstBridge();
	auto edge1 = std::make_pair(edge0.second, edge0.first);
	std::vector<std::vector<Polynomial>> rows;
	std::size_t size = faces.size() - 2;

	for (const auto &face : faces) {
		if (face.matches(edge0) || face.matches(edge1)) {
			continue;
		}
		std::vector<Polynomial> row(size, Polynomial::ZERO);
		for (const auto &pair : face.matrixRow) {
			if (indices.find(pair.first.cro) == indices.end()) {
				indices.emplace(pair.first.cro, indices.size());
			}
			row[indices[pair.first.cro]] = poly(pair.second);
		}
		rows.push_back(row);
	}

	ConstMatrix matrix(rows);
	return matrix.determinant().reduced();
}

}
