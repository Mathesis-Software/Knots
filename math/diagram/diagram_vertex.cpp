#include "diagram.h"

namespace KE { namespace TwoD {

Diagram::Vertex *Diagram::addVertex(Vertex* v, int x, int y) {
	if (!base) {
		base = new Vertex(x, y);
		return base;
	}

	Vertex *new_vertex = new Vertex(v ? v : base->prev(), x, y);

	const Edge removed(new_vertex->prev(), new_vertex->next());
	const Edge new1(new_vertex->prev(), new_vertex);
	const Edge new2(new_vertex, new_vertex->next());

	for (const Edge &edge : this->edges()) {
		auto removed_crossing = this->getCrossing(removed.start, edge.start);
		this->removeCrossing(removed.start, edge.start);

		for (const Edge new_edge : {new1, new2}) {
			if (edge.intersects(new_edge)) {
				if (removed_crossing && removed_crossing->up == removed) {
					this->addCrossing(edge.start, new_edge.start);
				} else {
					this->addCrossing(new_edge.start, edge.start);
				}
			}
		}
	}

	order();
	return new_vertex;
}

void Diagram::removeVertex(Vertex* v) {
	if (v == nullptr) {
		return;
	}

	if (base == base->next()) {
		delete base;
		base = nullptr;
		return;
	}

	if (v == base) {
		base = base->next();
	}

	v->exclude();

	const Edge removed1(v->prev(), v);
	const Edge removed2(v, v->next());
	const Edge merged(v->prev(), v->next());
	for (const Edge &edge : this->edges()) {
		auto removed_crossing1 = this->getCrossing(removed1.start, edge.start);
		auto removed_crossing2 = this->getCrossing(removed2.start, edge.start);
		this->removeCrossing(removed1.start, edge.start);
		this->removeCrossing(removed2.start, edge.start);

		if (!edge.intersects(merged)) {
			continue;
		}
		if ((removed_crossing1 && removed_crossing1->up == removed1) ||
				(removed_crossing2 && removed_crossing2->up == removed2)) {
			this->addCrossing(edge.start, merged.start);
		} else {
			this->addCrossing(merged.start, edge.start);
		}
	}

	delete v;
}

void Diagram::moveVertex(Vertex *v, int x, int y) {
	if (v == nullptr)
		return;

	v->moveTo(x, y);

	const Edge changed1(v->prev(), v);
	const Edge changed2(v, v->next());
	for (const Edge &edge : this->edges()) {
		auto changed_crossing1 = this->getCrossing(changed1.start, edge.start);
		auto changed_crossing2 = this->getCrossing(changed2.start, edge.start);

		if (edge.intersects(changed1)) {
			if (!changed_crossing1) {
				if (changed_crossing2 && changed_crossing2->up == changed2) {
					this->addCrossing(edge.start, changed1.start);
				} else {
					this->addCrossing(changed1.start, edge.start);
				}
			}
		} else {
			this->removeCrossing(edge.start, changed1.start);
		}

		if (edge.intersects(changed2)) {
			if (!changed_crossing2) {
				if (changed_crossing1 && changed_crossing1->up == changed1) {
					this->addCrossing(edge.start, changed2.start);
				} else {
					this->addCrossing(changed2.start, edge.start);
				}
			}
		} else {
			this->removeCrossing(edge.start, changed2.start);
		}
	}

	order();
}

}}
