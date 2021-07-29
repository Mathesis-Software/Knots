#include "diagram.h"

namespace KE { namespace TwoD {

bool Diagram::tryAddCrossing(vertex *v1, vertex *v2) {
	if (!crossed(v1, v2)) {
		return false;
	}

	new crossing (v1, v2);

	return true;
}

bool Diagram::tryChangeCrossing(vertex *v1, vertex *v2) {
	if (tryRemoveCrossing (v2, v1)) {
		return tryAddCrossing (v1, v2);
	}

	return false;
}

bool Diagram::tryRemoveCrossing(vertex *v1, vertex *v2) {
	for (crossing *c = v1->crs(); c; c = c->next()) {
		if (c->up() == v2) {
			delete c;
			return true;
		}
	}

	return false;
}

bool Diagram::isCrossing(vertex *v1, vertex *v2) {
	for (crossing *c = v1->crs(); c; c = c->next()) {
		if (c->up() == v2) {
			return true;
		}
	}

	return false;
}

}}
