#include <algorithm>
#include <cmath>
#include <functional>
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
	for (Edge &edge : this->edges()) {
		edge.orderCrossings();
	}
}

void Diagram::shift(int x, int y) {
	vertex *v = base;
	do {
		v->move(x, y);
		v = v->next();
	} while (v != base);
}

std::list<vertex*> Diagram::vertices() const {
	std::list<vertex*> vertices;
	vertex *v = this->base;
	if (v == nullptr) {
		return vertices;
	}

	do {
		vertices.push_back(v);
		v = v->next();
	} while (v != this->base);
	return vertices;
}

std::list<Diagram::Edge> Diagram::edges() const {
	std::list<Edge> edges;
	vertex *v = this->base;
	if (v == nullptr) {
		return edges;
	}

	for (; v->next() != this->base; v = v->next()) {
		edges.push_back(Edge(v, v->next()));
	}
	if (this->isClosed) {
		edges.push_back(Edge(v, v->next()));
	}
	return edges;
}

vertex *Diagram::findVertex(double x, double y, double maxDistance) const {
	double best = std::numeric_limits<double>::max();
	vertex *found = nullptr;

	for (auto vertex : this->vertices()) {
		const double distance = hypot(x - vertex->x(), y - vertex->y());
		if (distance < best) {
			best = distance;
			found = vertex;
		}
	}

	return best <= maxDistance ? found : nullptr;
}

std::shared_ptr<crossing> Diagram::findCrossing(double x, double y, double maxDistance) const {
	double best = std::numeric_limits<double>::max();
	std::shared_ptr<crossing> found;

	for (auto vertex : this->vertices()) {
		for (auto crs : vertex->crossings()) {
			const double distance = hypot(x - crs.x(), y - crs.y());
			if (distance < best && distance <= maxDistance) {
				best = distance;
				found = std::make_shared<crossing>(crs);
			}
		}
	}

	return found;
}

std::shared_ptr<Diagram::Edge> Diagram::findEdge(double x, double y, double maxDistance) const {
	double best = std::numeric_limits<double>::max();
	std::shared_ptr<Edge> found;

	for (const Edge &edge : this->edges()) {
		const double dx = edge.end->x() - edge.start->x();
		const double dy = edge.end->y() - edge.start->y();

		if (dx == 0 and dy == 0) {
			// the edge has zero length
			continue;
		}

		if (((x - edge.start->x()) * dx + (y - edge.start->y()) * dy < 0) ||
				((x - edge.end->x()) * dx + (y - edge.end->y()) * dy > 0)) {
			// (x, y) is outside of the perpendicular strip built on the edge segment
			continue;
		}

		const double distance = fabs((x - edge.start->x()) * dy - (y - edge.start->y()) * dx) / hypot(dx, dy);
		if (distance < best && distance <= maxDistance) {
			best = distance;
			found = std::make_shared<Edge>(edge);
		}
	}

	return found;
}

namespace {
	int orientation(const vertex &v0, const vertex &v1, const vertex &v2) {
		const int area = v0.x() * (v1.y() - v2.y()) + v1.x() * (v2.y() - v0.y()) + v2.x() * (v0.y() - v1.y());
		if (area < 0) {
			return -1;
		} else if (area > 0) {
			return 1;
		} else {
			return 0;
		}
	}
}

bool Diagram::Edge::intersects(const Diagram::Edge &edge) const {
	const int ori = orientation(*this->start, *edge.start, *this->end); 
	return
		ori != 0 &&
		ori == orientation(*edge.start, *this->end, *edge.end) &&
		ori == orientation(*this->end, *edge.end, *this->start) &&
		ori == orientation(*edge.end, *this->start, *edge.start);
}

void Diagram::Edge::orderCrossings() {
	std::function<bool(const crossing&,const crossing&)> comparator;
  if (abs(this->dx()) > abs(this->dy())) {
    if (this->dx() > 0) {
			comparator = [](const crossing &c0, const crossing &c1) { return c0.x() < c1.x(); };
		} else {
			comparator = [](const crossing &c0, const crossing &c1) { return c0.x() > c1.x(); };
		}
	} else {
    if (this->dy() > 0) {
			comparator = [](const crossing &c0, const crossing &c1) { return c0.y() < c1.y(); };
		} else {
			comparator = [](const crossing &c0, const crossing &c1) { return c0.y() > c1.y(); };
		}
	}
	this->start->_crossings.sort(comparator);
}

}}
