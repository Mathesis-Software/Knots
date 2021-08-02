#include "diagram.h"

namespace KE { namespace TwoD {

Diagram::Vertex *Diagram::addVertex(Vertex* v, int x, int y) {
  if (!base) {
    base = new Vertex(x, y);
    return base;
  }

  Vertex *new_vertex = new Vertex(v ? v : base->prev(), x, y);

  Vertex *current = base;
  do {
		const Edge e(current, current->next());
		const Edge e1(new_vertex->prev(), new_vertex);
		const Edge e2(new_vertex, new_vertex->next());

    if (e.intersects(e2)) {
      if (isCrossing(new_vertex->prev(), current))
        tryAddCrossing(new_vertex, current);
      else
        tryAddCrossing(current, new_vertex);
    }

    if (e.intersects(e1)) {
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

void Diagram::removeVertex(Vertex* v) {
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

  Vertex *current = base;

  do {
		const Edge e(current, current->next());
		const Edge e1(v->prev(), v);
    if (e.intersects(e1)) {
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

void Diagram::moveVertex(Vertex *v, int x, int y) {
  if (v == nullptr)
    return;

  v->moveTo(x, y);

  Vertex *current = base;

  do {
		const Edge e(current, current->next());
		const Edge e1(v->prev(), v);
		const Edge e2(v, v->next());

    switch ((e.intersects(e2) ? 1 : 0) + (e.intersects(e1) ? 2 : 0)) {
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

}}
