#include <cmath>
#include <limits>

#include "diagram.h"

namespace KE { namespace TwoD {

Diagram::Diagram() : caption("New Diagram"), base(nullptr), isClosed(false) {
}

Diagram::~Diagram() {
	this->clear();
}

void Diagram::clear() {
	while (base != nullptr) {
		removeVertex(base);
	}
	this->isClosed = false;
}

int Diagram::length() {
	vertex *v = base->next();
	int l;
	for (l = 1; v != base; l++) {
		v = v->next();
	}
	return l;
}

void Diagram::order() {
  vertex *v = base;
  do {
    v->order();
    v = v->next();
  } while (v != base);
}

void Diagram::shift(int x, int y) {
	vertex *v = base;
	do {
		v->move(x, y);
		v = v->next();
	} while (v != base);
}

vertex *Diagram::findVertex(double x, double y, double maxDistance) const {
  vertex *v = this->base;
  if (v == nullptr) {
    return nullptr;
	}

	double best = std::numeric_limits<double>::max();
	vertex *found = nullptr;

  do {
		const double distance = hypot(x - v->x(), y - v->y());
		if (distance < best) {
			best = distance;
			found = v;
		}
    v = v->next();
  } while (v != this->base);

  return best <= maxDistance ? found : nullptr;
}

crossing *Diagram::findCrossing(double x, double y, double maxDistance) const {
  vertex *v = this->base;
  if (v == nullptr) {
    return nullptr;
	}

	double best = std::numeric_limits<double>::max();
	crossing *found = nullptr;

  do {
    for (crossing *crs = v->crs(); crs != nullptr; crs = crs->next()) {
    	const double distance = hypot(x - crs->x(), y - crs->y());
			if (distance < best) {
				best = distance;
				found = crs;
			}
    }
    v = v->next();
  } while (v != this->base);

  return best <= maxDistance ? found : nullptr;
}

vertex *Diagram::findEdge(double x, double y, double maxDistance) const {
  vertex *v = this->base;
  if (v == nullptr) {
    return nullptr;
	}

	double best = std::numeric_limits<double>::max();
	vertex *found = nullptr;

  do {
    const double dx = v->next()->x() - v->x();
    const double dy = v->next()->y() - v->y();

		if (dx == 0 and dy == 0) {
			// the edge has zero length
			continue;
		}

    if (((x - v->x()) * dx + (y - v->y()) * dy < 0) ||
				((x - v->next()->x()) * dx + (y - v->next()->y()) * dy > 0)) {
			// (x, y) is outside of the perpendicular strip built on the edge segment
		 	continue;
		}

		const double distance = fabs((x - v->x()) * dy - (y - v->y() * dx)) / hypot(dx, dy);
		if (distance < best) {
			best = distance;
			found = v;
		}

    v = v->next();
  } while (isClosed ? (v != this->base) : (v->next() != this->base));

  return best <= maxDistance ? found : nullptr;
}

}}
