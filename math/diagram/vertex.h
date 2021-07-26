#ifndef __VERTEX_H__
#define __VERTEX_H__

class crossing;

class vertex {

  friend class crossing;

private:

  vertex *vertex_prev, *vertex_next;
  crossing *vertex_crs;
  int coord_x, coord_y;

public:

  vertex (int, int);
  vertex (vertex*, int, int);
  ~vertex (void);
  void exclude (void);

  void move (int, int);
  void moveTo (int, int);

  vertex *next (void);
  vertex *prev (void);
  crossing *crs (void);
  int x (void);
  int y (void);

  void order (void);
};

class crossing {

  friend class vertex;

private:

  vertex *arc_up, *arc_down;
  crossing *next_crossing;
  crossing *prev_crossing;
  void plus (void);

public:

  crossing (vertex*, vertex*);
  ~crossing (void);

  crossing *next (void);
  crossing *prev (void);
  vertex *up (void);
  vertex *down (void);
  float x (void);
  float y (void);
};

bool crossed (vertex*, vertex*);

inline vertex *vertex::next (void) {
  return vertex_next;
}
inline vertex *vertex::prev (void) {
  return vertex_prev;
}
inline crossing *vertex::crs (void) {
  return vertex_crs;
}

inline int vertex::x (void) {
  return coord_x;
}
inline int vertex::y (void) {
  return coord_y;
}

inline crossing *crossing::next (void) {
  return next_crossing;
}
inline crossing *crossing::prev (void) {
  return prev_crossing;
}

inline vertex *crossing::up (void) {
  return arc_up;
}
inline vertex *crossing::down (void) {
  return arc_down;
}

#endif /* __VERTEX_H__ */
