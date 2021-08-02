#include "diagram.h"

namespace KE { namespace TwoD {

void Diagram::addCrossing(Vertex *v1, Vertex *v2) {
	this->removeCrossing(v1, v2);

	Edge e1(v1, v1->next());
	const Edge e2(v2, v2->next());
	if (!e1.intersects(e2)) {
		return;
	}

	v1->_crossings.push_back(Crossing(v1, v2));
	e1.orderCrossings();
}

bool Diagram::tryChangeCrossing(Vertex *v1, Vertex *v2) {
	if (isCrossing(v2, v1)) {
		this->removeCrossing(v1, v2);
		this->addCrossing(v1, v2);
		return true;
	}

	return false;
}

void Diagram::removeCrossing(Vertex *v1, Vertex *v2) {
	v1->_crossings.remove(Crossing(v1, v2));
	v2->_crossings.remove(Crossing(v2, v1));
}

bool Diagram::isCrossing(Vertex *v1, Vertex *v2) {
	for (const auto &crs : v1->crossings()) {
		if (crs.up() == v2) {
			return true;
		}
	}

	return false;
}

}}
