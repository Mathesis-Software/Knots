#ifndef __VERTEX_H__
#define __VERTEX_H__

#include <list>

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

#endif /* __VERTEX_H__ */
