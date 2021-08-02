#include "diagram.h"

namespace KE { namespace TwoD {

bool Diagram::tryAddCrossing(Vertex *v1, Vertex *v2) {
	Edge e1(v1, v1->next());
	const Edge e2(v2, v2->next());
	if (!e1.intersects(e2)) {
		return false;
	}

	v1->_crossings.push_back(Crossing(v1, v2));
	e1.orderCrossings();

	return true;
}

bool Diagram::tryChangeCrossing(Vertex *v1, Vertex *v2) {
	if (isCrossing(v2, v1)) {
		tryRemoveCrossing(v2, v1);
		return tryAddCrossing(v1, v2);
	}

	return false;
}

void Diagram::tryRemoveCrossing(Vertex *v1, Vertex *v2) {
	v1->_crossings.remove(Crossing(v1, v2));
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
