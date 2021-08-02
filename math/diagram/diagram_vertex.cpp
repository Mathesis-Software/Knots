#include "diagram.h"

namespace KE { namespace TwoD {

Diagram::Vertex *Diagram::addVertex(Vertex* v, int x, int y) {
	if (!base) {
		base = new Vertex(x, y);
		return base;
	}

	Vertex *new_vertex = new Vertex(v ? v : base->prev(), x, y);

	for (const Edge &edge : this->edges()) {
		const Edge e1(new_vertex->prev(), new_vertex);
		const Edge e2(new_vertex, new_vertex->next());

		if (edge.intersects(e2)) {
			if (isCrossing(new_vertex->prev(), edge.start))
				this->addCrossing(new_vertex, edge.start);
			else
				this->addCrossing(edge.start, new_vertex);
		}

		if (edge.intersects(e1)) {
			if (!getCrossing(edge.start, new_vertex->prev())) {
				this->addCrossing(edge.start, new_vertex->prev());
			}
		} else {
			this->removeCrossing(edge.start, new_vertex->prev());
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
		// TODO: delete '.start' comparison; compare edges directly
		if ((removed_crossing1 && removed_crossing1->up().start == removed1.start) ||
				(removed_crossing2 && removed_crossing2->up() == removed2)) {
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

	for (const Edge &edge : this->edges()) {
		const Edge e1(v->prev(), v);
		const Edge e2(v, v->next());

		if (edge.intersects(e2)) {
			if (edge.intersects(e1)) {
				if (!getCrossing(edge.start, v)) {
					this->addCrossing(edge.start, v);
				}
				if (!getCrossing(edge.start, v->prev())) {
					this->addCrossing(edge.start, v->prev());
				}
			} else {
				if (!getCrossing(edge.start, v)) {
					if (isCrossing(v->prev(), edge.start))
						this->addCrossing(v, edge.start);
					else
						this->addCrossing(edge.start, v);
				}
				this->removeCrossing(edge.start, v->prev());
			}
		} else {
			if (edge.intersects(e1)) {
				if (!getCrossing(edge.start, v->prev())) {
					if (isCrossing(v, edge.start))
						this->addCrossing(v->prev(), edge.start);
					else
						this->addCrossing(edge.start, v->prev());
				}
				this->removeCrossing(edge.start, v);
			} else {
				this->removeCrossing(edge.start, v);
				this->removeCrossing(edge.start, v->prev());
			}
		}
	}

	order();
}

}}
