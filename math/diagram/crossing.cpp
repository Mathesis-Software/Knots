#include <stdlib.h>
#include "diagram.h"

crossing::crossing(vertex* v1, vertex *v2) {
  arc_up = v2;
  arc_down = v1;
  next_crossing = v1->crs();
  prev_crossing = nullptr;

  v1->vertex_crs = this;

  if (!next())
    return;

  next()->prev_crossing = this;

  if (abs(down()->x() - down()->next()->x()) > abs(down()->y() - down()->next()->y())) {
    if (down()->x() > down()->next()->x())
      while (next() && x() < next()->x())
        plus();
    else
      while (next() && x() > next()->x())
        plus();
  }
  else
  {
    if (down()->y() > down()->next()->y())
      while (next() && y() < next()->y())
        plus();
    else
      while (next() && y() > next()->y())
        plus();
  }
}

void crossing::plus() {
  if (!next())
    return;

  if (!prev())
    down()->vertex_crs = next();

  crossing *nn = next()->next();
  crossing *n = next();
  crossing *p = prev();

  if (nn)
    nn->prev_crossing = this;

  n->next_crossing = this;
  n->prev_crossing = p;

  next_crossing = nn;
  prev_crossing = n;

  if (p)
    p->next_crossing = n;
}

crossing::~crossing() {
  if (prev())
    prev_crossing->next_crossing = next();
  if (next())
    next_crossing->prev_crossing = prev();
  if (down()->crs() == this)
    arc_down->vertex_crs = next();
}

float crossing::x() {
  int d0, d1;

  d0 = (up()->next()->x() - up()->x())
     * (down()->y() - down()->next()->y())
     - (up()->next()->y() - up()->y())
     * (down()->x() - down()->next()->x());

  if (d0 == 0)
    return 0.0;
  
  d1 = (down()->x() - up()->x())
     * (down()->y() - down()->next()->y())
     - (down()->y() - up()->y())
     * (down()->x() - down()->next()->x());

  return up()->x() + 1.0 * (up()->next()->x() - up()->x()) * d1 / d0;
}     

float crossing::y() {
  int d0, d1;

  d0 = (up()->next()->x() - up()->x()) * (down()->y() - down()->next()->y())
     - (up()->next()->y() - up()->y()) * (down()->x() - down()->next()->x());

  if (d0 == 0)
    return 0.0;
  
  d1 = (down()->x() - up()->x())
     * (down()->y() - down()->next()->y())
     - (down()->y() - up()->y())
     * (down()->x() - down()->next()->x());

  return up()->y() + 1.0 * (up()->next()->y() - up()->y()) * d1 / d0;
}

bool crossed(vertex *v1, vertex *v2) {
  if (v1 == v2 || v1->prev() == v2 || v1->next() == v2) {
    return 0;
	}

  int det[3];

  det[0] = (v1->next()->x() - v1->x()) * (v2->y() - v2->next()->y())
         - (v1->next()->y() - v1->y()) * (v2->x() - v2->next()->x());
  if (det[0] == 0) {
    return 0;
	}

  det[1] = (v1->next()->x() - v1->x()) * (v2->y() - v1->y())
         - (v1->next()->y() - v1->y()) * (v2->x() - v1->x());
  det[2] = (v2->x() - v1->x()) * (v2->y() - v2->next()->y())
         - (v2->y() - v1->y()) * (v2->x() - v2->next()->x());

  if (det[0] < 0) {
    det[0] = - det[0];
    det[1] = - det[1];
    det[2] = - det[2];
  }

  if (det[1] < 0 || det[2] < 0 || det[1] > det [0] || det[2] > det [0]) {
    return 0;
	}

  return 1;
}
