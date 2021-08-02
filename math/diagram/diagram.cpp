#include <algorithm>
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

void Diagram::order() {
	for (Edge &edge : this->edges()) {
		edge.orderCrossings();
	}
}

void Diagram::shift(int x, int y) {
	Vertex *v = base;
	do {
		v->move(x, y);
		v = v->next();
	} while (v != base);
}

std::list<Diagram::Vertex*> Diagram::vertices() const {
	std::list<Vertex*> vertices;
	Vertex *v = this->base;
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
	Vertex *v = this->base;
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

Diagram::Vertex *Diagram::findVertex(const FloatPoint &pt, float maxDistance) const {
	float best = std::numeric_limits<float>::max();
	Vertex *found = nullptr;

	for (auto vertex : this->vertices()) {
		const float distance = pt.distance(vertex->coords());
		if (distance < best) {
			best = distance;
			found = vertex;
		}
	}

	return best <= maxDistance ? found : nullptr;
}

std::shared_ptr<Diagram::Crossing> Diagram::findCrossing(const FloatPoint &pt, float maxDistance) const {
	float best = std::numeric_limits<float>::max();
	std::shared_ptr<Crossing> found;

	for (auto vertex : this->vertices()) {
		for (auto crs : vertex->crossings()) {
			const auto coords = crs.coords();
			if (!coords) {
				continue;
			}
			const float distance = pt.distance(*coords);
			if (distance < best && distance <= maxDistance) {
				best = distance;
				found = std::make_shared<Crossing>(crs);
			}
		}
	}

	return found;
}

std::shared_ptr<Diagram::Edge> Diagram::findEdge(const FloatPoint &pt, float maxDistance) const {
	float best = std::numeric_limits<float>::max();
	std::shared_ptr<Edge> found;

	for (const Edge &edge : this->edges()) {
		const float dx = edge.dx();
		const float dy = edge.dy();

		if (dx == 0 and dy == 0) {
			// the edge has zero length
			continue;
		}

		if (((pt.x - edge.start->x()) * dx + (pt.y - edge.start->y()) * dy < 0) ||
				((pt.x - edge.end->x()) * dx + (pt.y - edge.end->y()) * dy > 0)) {
			// pt is outside of the perpendicular strip built on the edge segment
			continue;
		}

		const float distance = fabs((pt.x - edge.start->x()) * dy - (pt.y - edge.start->y()) * dx) / hypotf(dx, dy);
		if (distance < best && distance <= maxDistance) {
			best = distance;
			found = std::make_shared<Edge>(edge);
		}
	}

	return found;
}

namespace {
	int orientation(const Diagram::Vertex &v0, const Diagram::Vertex &v1, const Diagram::Vertex &v2) {
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
	std::function<bool(const Crossing&,const Crossing&)> comparator;
  if (abs(this->dx()) > abs(this->dy())) {
    if (this->dx() > 0) {
			comparator = [](const Crossing &c0, const Crossing &c1) { return c0.coords()->x < c1.coords()->x; };
		} else {
			comparator = [](const Crossing &c0, const Crossing &c1) { return c0.coords()->x > c1.coords()->x; };
		}
	} else {
    if (this->dy() > 0) {
			comparator = [](const Crossing &c0, const Crossing &c1) { return c0.coords()->y < c1.coords()->y; };
		} else {
			comparator = [](const Crossing &c0, const Crossing &c1) { return c0.coords()->y > c1.coords()->y; };
		}
	}
	this->start->_crossings.sort(comparator);
}

}}
