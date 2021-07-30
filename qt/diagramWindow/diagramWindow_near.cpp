#include <cmath>

#include "diagramWindow.h"

vertex *diagramWindow::nearToVertex(int x, int y) {
  vertex *v = this->diagram.base;
  if (v == nullptr) {
    return nullptr;
	}

  do {
    if (hypot(x - v->x(), y - v->y()) < 17) {
      return v;
		}
    v = v->next();
  } while (v != this->diagram.base);

  return nullptr;
}

crossing *diagramWindow::nearToCross(int x, int y) {
  vertex *v = this->diagram.base;
  if (v == nullptr) {
    return nullptr;
	}

  do {
    crossing *crs = v->crs();
    while (crs) {
    	if (hypot(x - crs->x(), y - crs->y()) < 17) {
        return crs;
			}
      crs = crs->next();
    }
    v = v->next();
  } while (v != this->diagram.base);

  return nullptr;
}

vertex *diagramWindow::nearToEdge(int x, int y) {
  vertex *v = this->diagram.base;
  if (v == nullptr) {
    return nullptr;
	}

  do {
    const int X = v->next()->x() - v->x();
    const int Y = v->next()->y() - v->y();

    if (((x - v->x()) * X + (y - v->y()) * Y >= 0) &&
				((x - v->next()->x()) * X + (y - v->next()->y()) * Y <= 0) &&
				(abs ((x - v->x()) * Y - (y - v->y()) * X) <= 5 * hypot(X, Y))) {
      return v;
		}

    v = v->next();
  } while (isClosed ? (v != this->diagram.base) : (v->next() != this->diagram.base));

  return nullptr;
}
