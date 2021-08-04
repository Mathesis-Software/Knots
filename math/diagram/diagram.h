#ifndef __DIAGRAM_H__
#define __DIAGRAM_H__

#include <cmath>
#include <list>
#include <memory>
#include <string>

#include <rapidjson/document.h>

namespace KE {

namespace ThreeD {

class Knot;

}

namespace TwoD {

struct FloatPoint {
	const float x, y;
	FloatPoint(float x, float y) : x(x), y(y) {
	}

	float distance(const FloatPoint &pt) const {
		return hypotf(x - pt.x, y - pt.y);
	}
};

class Diagram {

friend class ThreeD::Knot;

public:
	struct Crossing;

	class Vertex {

	friend class Diagram;

	private:
		std::list<Crossing> _crossings;
		int _x, _y;

	public:
		Vertex(int x, int y) : _x(x), _y(y) {}

		void move(int dx, int dy) { this->_x += dx; this->_y += dy; }
		void moveTo(int x, int y) { this->_x = x; this->_y = y; }

	public:
		const std::list<KE::TwoD::Diagram::Crossing> &crossings() const { return this->_crossings; }
		int x() const { return this->_x; }
		int y() const { return this->_y; }
		FloatPoint coords() const { return FloatPoint(this->_x, this->_y); }
	};

	struct Edge {
		friend class Diagram;

		const std::shared_ptr<Vertex> start;
		const std::shared_ptr<Vertex> end;

		Edge(const std::shared_ptr<Vertex> &start, const std::shared_ptr<Vertex> &end) : start(start), end(end) {
		}
		int dx() const { return this->end->x() - this->start->x(); }
		int dy() const { return this->end->y() - this->start->y(); }
		bool intersects(const Edge &edge) const;

		bool operator == (const Edge &edge) const { return this->start == edge.start && this->end == edge.end; }

	private:
		void orderCrossings() const;
	};

	struct Crossing {
		const Edge up, down;

		Crossing(const Edge &up, const Edge &down) : up(up), down(down) {}

		std::shared_ptr<FloatPoint> coords() const;

		bool operator == (const Crossing &crs) const { return this->up == crs.up && this->down == crs.down; }
	};

	public:
		std::string caption;

	private:
		std::list<std::shared_ptr<Vertex>> _vertices;
		bool _isClosed;

	public:
		bool isClosed() const { return this->_isClosed; }
		const std::list<std::shared_ptr<Vertex>> &vertices() const { return this->_vertices; }
		std::list<Edge> edges() const;

		std::shared_ptr<Vertex> addVertex(int x, int y);
		std::shared_ptr<Vertex> addVertex(const Edge &edge, int x, int y);
		void removeVertex(const std::shared_ptr<Vertex> &vertex);
		void moveVertex(const std::shared_ptr<Vertex> &vertex, int x, int y);
		void close();

		void flipCrossing(Crossing &crossing);
		void shift(int x, int y);
		bool simplify(std::size_t depth);
		void clear();

		std::shared_ptr<Vertex> findVertex(const FloatPoint &pt, float maxDistance) const;
		std::shared_ptr<Edge> findEdge(const FloatPoint &pt, float maxDistance) const;
		std::shared_ptr<Crossing> findCrossing(const FloatPoint &pt, float maxDistance) const;

	private:
		void addCrossing(const Edge &up, const Edge &down);
		void removeCrossing(const Edge &edge1, const Edge &edge2);
		std::shared_ptr<Crossing> getCrossing(const Edge &edge1, const Edge &edge2);
		void order();

	public:
		Diagram();
		Diagram(const rapidjson::Document &doc);
		~Diagram();

		rapidjson::Document save() const;

private:
	Diagram(const Diagram&) = delete;
	Diagram& operator=(const Diagram&) = delete;
};

}}

#endif /* __DIAGRAM_H__ */
