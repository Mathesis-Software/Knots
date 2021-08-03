#ifndef __DIAGRAM_H__
#define __DIAGRAM_H__

#include <cmath>
#include <iostream>
#include <list>
#include <memory>
#include <string>

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
		Vertex *vertex_prev, *vertex_next;
		std::list<Crossing> _crossings;
		int coord_x, coord_y;

	public:
		Vertex(int, int);
		Vertex(Vertex*, int, int);
		~Vertex();
		void exclude();

		void move(int, int);
		void moveTo(int, int);

	private:
		Vertex *next() const { return this->vertex_next; }
		Vertex *prev() const { return this->vertex_prev; }

	public:
		const std::list<KE::TwoD::Diagram::Crossing> &crossings() const { return this->_crossings; }
		int x() const { return this->coord_x; }
		int y() const { return this->coord_y; }
		FloatPoint coords() const { return FloatPoint(this->coord_x, this->coord_y); }
	};

	struct Edge {
		friend class Diagram;

		Vertex *start;
		Vertex *end;

		Edge(Vertex *start, Vertex *end) : start(start), end(end) {
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
		Vertex *base;

	public:
		bool isClosed;

	public:
		std::list<Vertex*> vertices() const;
		std::list<Edge> edges() const;
		Vertex *addVertex(Vertex*, int, int);
		void removeVertex(Vertex*);
		void moveVertex(Vertex*, int, int);
		void flipCrossing(Crossing &crossing);
		void shift(int x, int y);
		bool simplify(int depth);
		void clear();

		Vertex *findVertex(const FloatPoint &pt, float maxDistance) const;
		std::shared_ptr<Edge> findEdge(const FloatPoint &pt, float maxDistance) const;
		std::shared_ptr<Crossing> findCrossing(const FloatPoint &pt, float maxDistance) const;

	private:
		void addCrossing(const Edge &up, const Edge &down);
		void removeCrossing(const Edge &edge1, const Edge &edge2);
		std::shared_ptr<Crossing> getCrossing(const Edge &edge1, const Edge &edge2);
		void order();

	public:
		Diagram();
		Diagram(std::istream &is);
		~Diagram();

		void save(std::ostream &os);

private:
	Diagram(const Diagram&) = delete;
	Diagram& operator=(const Diagram&) = delete;
};

}}

#endif /* __DIAGRAM_H__ */
