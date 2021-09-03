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
#include <iostream>
#include <fstream>
#include <list>
#include <map>
#include <set>
#include <vector>

#include <rapidjson/document.h>
#include <rapidjson/istreamwrapper.h>

#include "../ke/Util_rapidjson.h"
#include "../ke/Diagram.h"
#include "../math/Polynomial.h"

using namespace KE::TwoD;

namespace {

template<typename T>
class SquareMatrix {

public:
	virtual ~SquareMatrix() {}

	virtual std::size_t dimension() const = 0;
	virtual const T &at(std::size_t i, std::size_t j) const = 0;

	T determinant() const;
};

template<typename T>
class ConstMatrix : public SquareMatrix<T> {

private:
	const std::vector<std::vector<T>> elements;

public:
	ConstMatrix(const std::vector<std::vector<T>> &elements) : elements(elements) {
	}

	std::size_t dimension() const override {
		return this->elements.size();
	}
	const T &at(std::size_t i, std::size_t j) const override {
		return this->elements[i][j];
	}
};

template<typename T>
class SliceMatrix : public SquareMatrix<T> {

private:
	const SquareMatrix<T> &base;
	const std::size_t row, column;

public:
	SliceMatrix(const SquareMatrix<T> &base, std::size_t row, std::size_t column) : base(base), row(row), column(column) {
	}

	std::size_t dimension() const override {
		return this->base.dimension() - 1;
	}
	const T &at(std::size_t i, std::size_t j) const override {
		if (i >= this->row) {
			i += 1;
		}
		if (j >= this->column) {
			j += 1;
		}
		return this->base.at(i, j);
	}
};

template<typename T>
T SquareMatrix<T>::determinant() const {
	const std::size_t dim = this->dimension();
	if (dim == 1) {
		return this->at(0, 0);
	} else {
		T sum;
		for (std::size_t i = 0; i < dim; i += 1) {
			T coef = this->at(i, 0);
			if (coef != 0) {
				if (i % 2) {
					coef *= -1;
				}
				sum += coef * SliceMatrix<T>(*this, i, 0).determinant();
			}
		}
		return sum;
	}
}

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
		auto &crossings = edge2Crossings[edge];
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

Math::Polynomial poly(Face::Role role) {
	switch (role) {
		default:
			return Math::Polynomial::ZERO;
		case Face::leftBefore:
			return Math::Polynomial::MINUS_T;
		case Face::rightBefore:
			return Math::Polynomial::ONE;
		case Face::leftAfter:
			return Math::Polynomial::T;
		case Face::rightAfter:
			return Math::Polynomial::MINUS_ONE;
	}
}

Math::Polynomial alexanderPolynomial(const Diagram &diagram) {
	if (!diagram.hasCrossings()) {
		return Math::Polynomial::ONE;
	}

	const auto faces = collectFaces(diagram);

	std::map<Diagram::Crossing,int> indices;

	auto edge0 = faces[0].edges()[0];
	auto edge1 = std::make_pair(edge0.second.inversion(), edge0.first.inversion());
	std::vector<std::vector<Math::Polynomial>> rows;
	std::size_t size = faces.size() - 2;

	for (const auto &face : faces) {
		const auto edges = face.edges();
		if (std::find(edges.begin(), edges.end(), edge0) != edges.end() || std::find(edges.begin(), edges.end(), edge1) != edges.end()) {
			continue;
		}
		std::vector<Math::Polynomial> row(size, Math::Polynomial::ZERO);
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

int main(int argc, const char **argv) {
	if (argc != 2) {
		std::cerr << "Usage:\n\t" << argv[0] << " <file.dgr>\n";
		return 1;
	}

	rapidjson::Document doc;
	std::ifstream is(argv[1]);
	rapidjson::IStreamWrapper wrapper(is);
	doc.ParseStream(wrapper);
	is.close();
	Diagram diagram(doc);

	std::cout << alexanderPolynomial(diagram) << "\n";

	return 0;
}
