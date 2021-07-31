#ifndef __VERTEX_H__
#define __VERTEX_H__

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

public:
  crossing *crs();
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

  crossing *next();
  crossing *prev();
  vertex *up();
  vertex *down();
  float x();
  float y();
};

inline crossing *vertex::crs() {
  return vertex_crs;
}

inline crossing *crossing::next() {
  return next_crossing;
}
inline crossing *crossing::prev() {
  return prev_crossing;
}

inline vertex *crossing::up() {
  return arc_up;
}
inline vertex *crossing::down() {
  return arc_down;
}

#endif /* __VERTEX_H__ */
