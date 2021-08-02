#include "diagram.h"

namespace KE { namespace TwoD {

void Diagram::addCrossing(const Edge &up, const Edge &down) {
	this->removeCrossing(up, down);

	if (!up.intersects(down)) {
		return;
	}

	down.start->_crossings.push_back(Crossing(up, down));
	down.orderCrossings();
}

void Diagram::flipCrossing(Crossing &crs) {
	this->addCrossing(crs.down, crs.up);
}

void Diagram::removeCrossing(const Edge &edge1, const Edge &edge2) {
	edge1.start->_crossings.remove(Crossing(edge2, edge1));
	edge2.start->_crossings.remove(Crossing(edge1, edge2));
}

std::shared_ptr<Diagram::Crossing> Diagram::getCrossing(const Edge &edge1, const Edge &edge2) {
	for (const auto &crs : edge1.start->crossings()) {
		if (crs.up == edge2) {
			return std::make_shared<Crossing>(crs);
		}
	}
	for (const auto &crs : edge2.start->crossings()) {
		if (crs.up == edge1) {
			return std::make_shared<Crossing>(crs);
		}
	}

	return nullptr;
}

}}
