#include <functional>

#include "diagram.h"

namespace KE { namespace TwoD {

std::list<Diagram::Crossing> Diagram::crossings(const Edge &edge, bool includeUp) const {
	if (!includeUp) {
		return edge.start->crossings;
	}

	auto list = edge.start->crossings;
	for (const auto &e : this->edges()) {
		if (e == edge) {
			continue;
		}
		for (const auto &crs : e.start->crossings) {
			if (crs.up == edge) {
				list.push_back(crs);
			}
		}
	}
	edge.orderCrossings(list);
	return list;
}

void Diagram::addCrossing(const Edge &up, const Edge &down) {
	this->removeCrossing(up, down);

	if (!up.intersects(down)) {
		return;
	}

	down.start->crossings.push_back(Crossing(up, down));
	down.orderCrossings(down.start->crossings);
}

void Diagram::flipCrossing(Crossing &crs) {
	this->addCrossing(crs.down, crs.up);
}

void Diagram::removeCrossing(const Edge &edge1, const Edge &edge2) {
	edge1.start->crossings.remove(Crossing(edge2, edge1));
	edge2.start->crossings.remove(Crossing(edge1, edge2));
}

std::shared_ptr<Diagram::Crossing> Diagram::getCrossing(const Edge &edge1, const Edge &edge2) {
	for (const auto &crs : this->crossings(edge1, false)) {
		if (crs.up == edge2) {
			return std::make_shared<Crossing>(crs);
		}
	}
	for (const auto &crs : this->crossings(edge2, false)) {
		if (crs.up == edge1) {
			return std::make_shared<Crossing>(crs);
		}
	}

	return nullptr;
}

void Diagram::Edge::orderCrossings(std::list<Crossing> &crossings) const {
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
	crossings.sort(comparator);
}

}}
