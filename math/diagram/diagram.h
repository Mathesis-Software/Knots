#ifndef __DIAGRAM_H__
#define __DIAGRAM_H__

#include <iostream>
#include <list>
#include <memory>
#include <string>

struct crossing;

namespace KE { namespace TwoD {

class Diagram;

}}

class vertex {

friend struct crossing;
friend class KE::TwoD::Diagram;

private:
  vertex *vertex_prev, *vertex_next;
	std::list<crossing> _crossings;
  int coord_x, coord_y;

public:
  vertex(int, int);
  vertex(vertex*, int, int);
  ~vertex();
  void exclude();

  void move(int, int);
  void moveTo(int, int);

private:
  vertex *next() const { return this->vertex_next; }
  vertex *prev() const { return this->vertex_prev; }

public:
	const std::list<crossing> &crossings() const { return this->_crossings; }
  int x() const { return this->coord_x; }
  int y() const { return this->coord_y; }
};

struct crossing {

friend class vertex;

private:
  vertex *arc_up, *arc_down;

public:
  crossing(vertex*, vertex*);

public:
  vertex *up() const { return this->arc_up; }
  vertex *down() const { return this->arc_down; }
  float x() const;
  float y() const;

	bool operator == (const crossing &crs) const { return this->arc_up == crs.arc_up && this->arc_down == crs.arc_down; }
};

namespace KE {

namespace ThreeD {

class Knot;

}

namespace TwoD {

class Diagram {

friend class ThreeD::Knot;

public:
	struct Edge {
		friend class Diagram;

		vertex *start;
		vertex *end;

		Edge(vertex *start, vertex *end) : start(start), end(end) {
		}
		int dx() const { return this->end->x() - this->start->x(); }
		int dy() const { return this->end->y() - this->start->y(); }
		bool intersects(const Edge &edge) const; 

	private:
		void orderCrossings();
	};

public:
	std::string caption;

private:
	vertex *base;

public:
	bool isClosed;

public:
	std::list<vertex*> vertices() const;
	std::list<Edge> edges() const;
	vertex *addVertex(vertex*, int, int);
	void removeVertex(vertex*);
	void moveVertex(vertex*, int, int);
	bool tryChangeCrossing(vertex*, vertex*);
	bool isCrossing(vertex*, vertex*);
	void shift(int x, int y);
	bool simplify(int depth);
	void clear();
	int length();

	vertex *findVertex(double x, double y, double maxDistance) const;
	std::shared_ptr<Edge> findEdge(double x, double y, double maxDistance) const;
	std::shared_ptr<crossing> findCrossing(double x, double y, double maxDistance) const;

private:
	bool tryAddCrossing(vertex*, vertex*);
	void tryRemoveCrossing(vertex*, vertex*);
	void order();

public:
	Diagram();
	Diagram(std::istream &is);
	~Diagram();

	bool isEmpty();
	
	void save(std::ostream &os);

private:
	Diagram(const Diagram&) = delete;
	Diagram& operator=(const Diagram&) = delete;
};

inline bool Diagram::isEmpty() {
	return this->base == nullptr;
}

}}

#endif /* __DIAGRAM_H__ */
