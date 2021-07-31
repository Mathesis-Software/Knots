#include "diagram.h"

namespace KE { namespace TwoD {

vertex *Diagram::addVertex(vertex* v, int x, int y) {
  if (!base) {
    base = new vertex(x, y);
    return base;
  }

  vertex *new_vertex = new vertex(v ? v : base->prev(), x, y);

  vertex *current = base;
  
  do {
    if (crossed(current, new_vertex)) {
      if (isCrossing(new_vertex->prev(), current))
        tryAddCrossing(new_vertex, current);
      else
        tryAddCrossing(current, new_vertex);
    }

    if (crossed(current, new_vertex->prev())) {
      if (!isCrossing(current, new_vertex->prev()) &&
          !isCrossing(new_vertex->prev(), current)) {
        tryAddCrossing(current, new_vertex->prev());
			}
    } else {
      if (isCrossing(current, new_vertex->prev()))
        tryRemoveCrossing(current, new_vertex->prev());
      else if (isCrossing(new_vertex->prev(), current))
        tryRemoveCrossing(new_vertex->prev(), current);
    }

    current = current->next();
  } while (current != base);

  order();
	return new_vertex;
}

void Diagram::removeVertex(vertex* v) {
  if (v == nullptr)
    return;

  if (base == base->next()) {
    delete base;
    base = nullptr;
    return;
  }

  if (v == base)
    base = base->next();

  v->exclude();

  vertex *current = base;
  
  do {
    if (crossed(current, v->prev())) {
      if (!isCrossing(v->prev(), current) && !isCrossing(current, v->prev())) {
        if (isCrossing(v, current))
          tryAddCrossing(v->prev(), current);
        else
          tryAddCrossing(current, v->prev());
      }
    } else {
      tryRemoveCrossing(current, v->prev());
      tryRemoveCrossing(v->prev(), current);
    }

    tryRemoveCrossing(current, v);
    current = current->next();
  } while (current != base);

  order();

  delete v;
}

void Diagram::moveVertex(vertex *v, int x, int y) {
  if (v == nullptr)
    return;

  v->moveTo(x, y);

  vertex *current = base;
  
  do {
    switch ((crossed(current, v) ? 1 : 0) + (crossed(current, v->prev()) ? 2 : 0)) {
      case 0:
        tryRemoveCrossing(current, v);
        tryRemoveCrossing(v, current);
        tryRemoveCrossing(current, v->prev());
        tryRemoveCrossing(v->prev(), current);
        break;
      case 1:
        if (!isCrossing(current, v) && !isCrossing(v, current)) {
					if (isCrossing(v->prev(), current))
						tryAddCrossing(v, current);
					else
						tryAddCrossing(current, v);
				}
        tryRemoveCrossing(current, v->prev());
        tryRemoveCrossing(v->prev(), current);
        break;
      case 2:
        if (!isCrossing(current, v->prev()) && !isCrossing(v->prev(), current)) {
					if (isCrossing(v, current))
            tryAddCrossing(v->prev(), current);
					else
            tryAddCrossing(current, v->prev());
				}
        tryRemoveCrossing(current, v);
        tryRemoveCrossing(v, current);
        break;
      case 3:
        if (!isCrossing(current, v) && !isCrossing(v, current)) {
          tryAddCrossing(current, v);
				}
        if (!isCrossing(current, v->prev()) && !isCrossing(v->prev(), current)) {
          tryAddCrossing(current, v->prev());
				}
        break;
    }

    current = current->next();
  } while (current != base);

  order();
}

int Diagram::numByV(vertex *v) {
  vertex *curr = base;
  int num;
  for (num = 0; curr != v; curr = curr->next()) {
    if (curr == base && num > 0)
      return -1;
    num++;
  }

  return num;
}

vertex *Diagram::vByNum(int num) {
  vertex *v = base;
  for (; num > 0; num --)
    v = v->next();

  return v;
}

}}
