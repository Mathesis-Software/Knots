#ifndef __DIAGRAM_H__
#define __DIAGRAM_H__

#include <iostream>
#include <list>
#include <memory>
#include <string>

namespace KE {

namespace ThreeD {

class Knot;

}

namespace TwoD {

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

	private:
		void orderCrossings();
	};

	struct Crossing {

	friend class Vertex;

	private:
		Vertex *arc_up, *arc_down;

	public:
		Crossing(Vertex*, Vertex*);

	public:
		Vertex *up() const { return this->arc_up; }
		Vertex *down() const { return this->arc_down; }
		float x() const;
		float y() const;

		bool operator == (const Crossing &crs) const { return this->arc_up == crs.arc_up && this->arc_down == crs.arc_down; }
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

	Vertex *findVertex(double x, double y, double maxDistance) const;
	std::shared_ptr<Edge> findEdge(double x, double y, double maxDistance) const;
	std::shared_ptr<Crossing> findCrossing(double x, double y, double maxDistance) const;

private:
	void addCrossing(Vertex*, Vertex*);
	void removeCrossing(Vertex*, Vertex*);
	bool isCrossing(Vertex*, Vertex*);
	std::shared_ptr<Crossing> getCrossing(Vertex*, Vertex*);
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
