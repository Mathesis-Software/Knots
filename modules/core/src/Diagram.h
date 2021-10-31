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

#ifndef __DIAGRAM_H__
#define __DIAGRAM_H__

#include <cmath>
#include <functional>
#include <limits>
#include <list>
#include <map>
#include <memory>
#include <string>

#include <rapidjson/document.h>

namespace KE::TwoD {

struct FloatPoint {
	const float x, y;
	FloatPoint(float x, float y) : x(x), y(y) {
	}

	float distance(const FloatPoint &pt) const {
		return hypotf(x - pt.x, y - pt.y);
	}
};

class Diagram {

public:
	struct Crossing;

	class Vertex {

		friend class Diagram;

	private:
		std::list<Crossing> crossings;
		int _x, _y;

	public:
		const std::size_t index;

	public:
		Vertex(int x, int y, std::size_t index) : _x(x), _y(y), index(index) {}

		void move(int dx, int dy) {
			this->_x += dx;
			this->_y += dy;
		}
		void moveTo(int x, int y) {
			this->_x = x;
			this->_y = y;
		}

		FloatPoint coords() const { return FloatPoint(this->_x, this->_y); }

	private:
		Vertex(const Vertex &) = delete;
		Vertex &operator=(const Vertex &) = delete;
	};

	struct Edge {

		friend class Diagram;

		const std::shared_ptr<Vertex> start;
		const std::shared_ptr<Vertex> end;

		Edge(const std::shared_ptr<Vertex> &start, const std::shared_ptr<Vertex> &end) : start(start), end(end) {
		}
		float dx() const { return this->end->coords().x - this->start->coords().x; }
		float dy() const { return this->end->coords().y - this->start->coords().y; }
		bool intersects(const Edge &edge) const;

		bool operator==(const Edge &edge) const { return this->start == edge.start && this->end == edge.end; }
		// for using in maps
		bool operator<(const Edge &edge) const { return this->start < edge.start || (this->start == edge.start && this->end < edge.end); }

	private:
		void orderCrossings(std::list<Crossing> &crossings) const;
	};

	struct Crossing {
		const Edge up, down;

		Crossing(const Edge &up, const Edge &down) : up(up), down(down) {}

		std::shared_ptr<FloatPoint> coords() const;

		bool operator==(const Crossing &crs) const { return this->up == crs.up && this->down == crs.down; }
		// for using in maps
		bool operator<(const Crossing &crs) const { return this->up < crs.up || (this->up == crs.up && this->down < crs.down); }
	};

public:
	std::string caption;

private:
	std::list<std::shared_ptr<Vertex>> _vertices;
	bool _isClosed;

public:
	Diagram();
	Diagram(const rapidjson::Value &doc);

	rapidjson::Document serialize() const;

	void clear();
	void close();
	bool isClosed() const { return this->_isClosed; }

	const std::list<std::shared_ptr<Vertex>> &vertices() const { return this->_vertices; }
	std::list<Edge> edges() const;
	const std::list<Crossing> &underCrossings(const Edge &edge) const { return edge.start->crossings; }
	std::map<Diagram::Edge, std::list<Diagram::Crossing>> allCrossings() const;
	bool hasCrossings() const;

	std::shared_ptr<Vertex> addVertex(int x, int y);
	std::shared_ptr<Vertex> addVertex(const Edge &edge, int x, int y);
	void removeVertex(const std::shared_ptr<Vertex> &vertex);
	// returns true if some crossings were changed
	bool moveVertex(const std::shared_ptr<Vertex> &vertex, int x, int y);

	void removeEdge(const Edge &edge);

	std::shared_ptr<Crossing> getCrossing(const Edge &edge1, const Edge &edge2);
	std::shared_ptr<Crossing> flipCrossing(Crossing &crossing);

	void shift(int dx, int dy);
	bool simplify(std::size_t depth);

	std::shared_ptr<Vertex> findVertex(const FloatPoint &pt, float maxDistance) const;
	std::shared_ptr<Edge> findEdge(const FloatPoint &pt, float maxDistance) const;
	std::shared_ptr<Crossing> findCrossing(const FloatPoint &pt, float maxDistance) const;

private:
	std::shared_ptr<Vertex> addVertex(int x, int y, std::size_t index);
	std::shared_ptr<Crossing> addCrossing(const Edge &up, const Edge &down);
	// returns true if the crossing has been removed
	bool removeCrossing(const Edge &edge1, const Edge &edge2);
	void order();

private:
	Diagram(const Diagram &) = delete;
	Diagram &operator=(const Diagram &) = delete;
};

}// namespace KE::TwoD

#endif /* __DIAGRAM_H__ */
