#include "diagram.h"

namespace KE { namespace TwoD {

Diagram::Vertex *Diagram::addVertex(int x, int y) {
	if (!base) {
		base = new Vertex(x, y);
		return base;
	} else {
		return this->addVertex(Edge(base->prev(), base), x, y);
	}
}

Diagram::Vertex *Diagram::addVertex(const Edge &edge, int x, int y) {
	Vertex *new_vertex = new Vertex(edge.start, x, y);

	const Edge new1(edge.start, new_vertex);
	const Edge new2(new_vertex, edge.end);

	for (const Edge &e : this->edges()) {
		auto removed_crossing = this->getCrossing(edge, e);
		this->removeCrossing(edge, e);

		for (const Edge new_edge : {new1, new2}) {
			if (e.intersects(new_edge)) {
				if (removed_crossing && removed_crossing->up == e) {
					this->addCrossing(e, new_edge);
				} else {
					this->addCrossing(new_edge, e);
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
		auto removed_crossing1 = this->getCrossing(removed1, edge);
		auto removed_crossing2 = this->getCrossing(removed2, edge);
		this->removeCrossing(removed1, edge);
		this->removeCrossing(removed2, edge);

		if (!edge.intersects(merged)) {
			continue;
		}
		if ((removed_crossing1 && removed_crossing1->up == edge) ||
				(removed_crossing2 && removed_crossing2->up == edge)) {
			this->addCrossing(edge, merged);
		} else {
			this->addCrossing(merged, edge);
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
		auto changed_crossing1 = this->getCrossing(changed1, edge);
		auto changed_crossing2 = this->getCrossing(changed2, edge);

		if (edge.intersects(changed1)) {
			if (!changed_crossing1) {
				if (changed_crossing2 && changed_crossing2->up == edge) {
					this->addCrossing(edge, changed1);
				} else {
					this->addCrossing(changed1, edge);
				}
			}
		} else {
			this->removeCrossing(edge, changed1);
		}

		if (edge.intersects(changed2)) {
			if (!changed_crossing2) {
				if (changed_crossing1 && changed_crossing1->up == edge) {
					this->addCrossing(edge, changed2);
				} else {
					this->addCrossing(changed2, edge);
				}
			}
		} else {
			this->removeCrossing(edge, changed2);
		}
	}

	order();
}

}}
