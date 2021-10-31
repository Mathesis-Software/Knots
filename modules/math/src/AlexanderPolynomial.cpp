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

#include <map>
#include <set>

#include <Diagram.h>

#include "DiagramProperty.h"
#include "Polynomial.h"
#include "SquareMatrix.h"

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
	bool operator==(const CrossingEx &ex) const {
		return this->cro == ex.cro && this->over == ex.over;
	}
	bool operator!=(const CrossingEx &ex) const {
		return !(*this == ex);
	}
	bool operator<(const CrossingEx &ex) const {
		return this->cro < ex.cro || (this->cro == ex.cro && this->over < ex.over);
	}
};

struct Bridge {
	const CrossingEx start;
	const CrossingEx end;
	const bool forward;

	Bridge(const CrossingEx &start, const CrossingEx &end, bool forward) : start(start), end(end), forward(forward) {}

	bool operator==(const Bridge &bridge) const {
		return this->start == bridge.start && this->end == bridge.end && this->forward == bridge.forward;
	}
	bool operator<(const Bridge &bridge) const {
		if (this->start < bridge.start) {
			return true;
		} else if (this->start != bridge.start) {
			return false;
		} else if (this->end < bridge.end) {
			return true;
		} else if (this->end != bridge.end) {
			return false;
		} else {
			return this->forward < bridge.forward;
		}
	}

	Bridge inversion() const {
		return Bridge(this->end, this->start, !this->forward);
	}
};

struct Face {
	std::list<Bridge> bridges;

	bool matches(const Bridge &bridge) const {
		return std::find(this->bridges.begin(), this->bridges.end(), bridge) != this->bridges.end();
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
	std::map<CrossingEx, CrossingEx> next;
	std::map<CrossingEx, CrossingEx> prev;
	for (std::size_t i = 0; i < all.size() - 1; i += 1) {
		next.emplace(all[i], all[i + 1]);
		prev.emplace(all[i + 1], all[i]);
	}
	next.emplace(all.back(), all.front());
	prev.emplace(all.front(), all.back());

	std::map<Diagram::Crossing, int> indices;
	std::set<Bridge> allBridges;
	for (const auto &crs : all) {
		allBridges.emplace(prev.at(crs), crs, true);
		allBridges.emplace(next.at(crs), crs, false);
	}

	std::vector<Face> faces;
	while (!allBridges.empty()) {
		Face face;
		face.bridges.push_back(*allBridges.begin());
		while (true) {
			const auto &last = face.bridges.back();
			allBridges.extract(last);
			const auto inv = last.end.inversion();
			const bool forward = last.forward == last.end.clockwise();
			const Bridge b(inv, forward ? next.at(inv) : prev.at(inv), forward);
			if (b == face.bridges.front()) {
				break;
			}
			face.bridges.push_back(b);
		}
		faces.push_back(face);
	}

	return faces;
}

Polynomial poly(const Bridge &bridge) {
	enum RoleOfBridgeEnd {
		LEFT_BEFORE,
		RIGHT_BEFORE,
		LEFT_AFTER,
		RIGHT_AFTER
	} role;

	if (bridge.forward) {
		role = bridge.end.over ? RIGHT_BEFORE : (bridge.end.clockwise() ? RIGHT_AFTER : LEFT_BEFORE);
	} else {
		role = bridge.end.over ? LEFT_AFTER : (bridge.end.clockwise() ? LEFT_BEFORE : RIGHT_AFTER);
	}

	switch (role) {
		default:
		case LEFT_BEFORE:
			return Polynomial::MINUS_T;
		case RIGHT_BEFORE:
			return Polynomial::ONE;
		case LEFT_AFTER:
			return Polynomial::T;
		case RIGHT_AFTER:
			return Polynomial::MINUS_ONE;
	}
}

}// namespace

bool AlexanderPolynomial::isApplicable(const Diagram &diagram) const {
	return diagram.isClosed();
}

Polynomial AlexanderPolynomial::value(const Diagram &diagram) const {
	if (!diagram.hasCrossings()) {
		return Polynomial::ONE;
	}

	const auto faces = collectFaces(diagram);

	std::map<Diagram::Crossing, int> indices;

	const auto skip = faces[0].bridges.front().inversion();
	std::vector<std::vector<Polynomial>> rows;
	std::size_t size = faces.size() - 2;

	bool first = true;
	for (const auto &face : faces) {
		if (first || face.matches(skip)) {
			first = false;
			continue;
		}
		std::vector<Polynomial> row(size, Polynomial::ZERO);
		for (const auto &bridge : face.bridges) {
			if (indices.find(bridge.end.cro) == indices.end()) {
				indices.emplace(bridge.end.cro, indices.size());
			}
			row[indices.at(bridge.end.cro)] = poly(bridge);
		}
		rows.push_back(row);
	}

	ConstMatrix matrix(rows);
	return matrix.determinant().reduced();
}

}// namespace KE::TwoD::Math
