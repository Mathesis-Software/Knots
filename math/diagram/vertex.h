#ifndef __VERTEX_H__
#define __VERTEX_H__

#include <list>

class crossing;

namespace KE { namespace TwoD {

class Diagram;

}}

class vertex {

friend class crossing;
friend class KE::TwoD::Diagram;

private:
  vertex *vertex_prev, *vertex_next;
  crossing *vertex_crs;
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
  crossing *crs() const { return this->vertex_crs; }

public:
	std::list<crossing*> crossings() const;
  int x() const { return this->coord_x; }
  int y() const { return this->coord_y; }

  void order();
};

class crossing {

friend class vertex;

private:
  vertex *arc_up, *arc_down;
  crossing *next_crossing;
  crossing *prev_crossing;
  void plus();

public:
  crossing(vertex*, vertex*);
  ~crossing();

private:
  crossing *next() const { return this->next_crossing; }
  crossing *prev() const { return this->prev_crossing; }

public:
  vertex *up();
  vertex *down();
  float x();
  float y();
};

inline vertex *crossing::up() {
  return arc_up;
}
inline vertex *crossing::down() {
  return arc_down;
}

#endif /* __VERTEX_H__ */
