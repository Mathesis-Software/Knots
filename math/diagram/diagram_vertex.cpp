#include "diagram.h"

namespace KE { namespace TwoD {

Diagram::Vertex *Diagram::addVertex(Vertex* v, int x, int y) {
	if (!base) {
		base = new Vertex(x, y);
		return base;
	}

	Vertex *new_vertex = new Vertex(v ? v : base->prev(), x, y);

	Vertex *current = base;
	do {
		const Edge e(current, current->next());
		const Edge e1(new_vertex->prev(), new_vertex);
		const Edge e2(new_vertex, new_vertex->next());

		if (e.intersects(e2)) {
			if (isCrossing(new_vertex->prev(), current))
				this->addCrossing(new_vertex, current);
			else
				this->addCrossing(current, new_vertex);
		}

		if (e.intersects(e1)) {
			if (!getCrossing(current, new_vertex->prev())) {
				this->addCrossing(current, new_vertex->prev());
			}
		} else {
			this->removeCrossing(current, new_vertex->prev());
		}

		current = current->next();
	} while (current != base);

	order();
	return new_vertex;
}

void Diagram::removeVertex(Vertex* v) {
	if (v == nullptr)
		return;

	if (base == base->next()) {
		delete base;
		base = nullptr;
		return;
	}

	if (v == base)
		base = base->next();

	v->exclude();

	Vertex *current = base;

	do {
		const Edge e(current, current->next());
		const Edge e1(v->prev(), v);
		if (e.intersects(e1)) {
			if (!getCrossing(v->prev(), current)) {
				if (isCrossing(v, current))
					this->addCrossing(v->prev(), current);
				else
					this->addCrossing(current, v->prev());
			}
		} else {
			this->removeCrossing(current, v->prev());
		}

		this->removeCrossing(current, v);
		current = current->next();
	} while (current != base);

	order();

	delete v;
}

void Diagram::moveVertex(Vertex *v, int x, int y) {
	if (v == nullptr)
		return;

	v->moveTo(x, y);

	Vertex *current = base;

	do {
		const Edge e(current, current->next());
		const Edge e1(v->prev(), v);
		const Edge e2(v, v->next());

		if (e.intersects(e2)) {
			if (e.intersects(e1)) {
				if (!getCrossing(current, v)) {
					this->addCrossing(current, v);
				}
				if (!getCrossing(current, v->prev())) {
					this->addCrossing(current, v->prev());
				}
			} else {
				if (!getCrossing(current, v)) {
					if (isCrossing(v->prev(), current))
						this->addCrossing(v, current);
					else
						this->addCrossing(current, v);
				}
				this->removeCrossing(current, v->prev());
			}
		} else {
			if (e.intersects(e1)) {
				if (!getCrossing(current, v->prev())) {
					if (isCrossing(v, current))
						this->addCrossing(v->prev(), current);
					else
						this->addCrossing(current, v->prev());
				}
				this->removeCrossing(current, v);
			} else {
				this->removeCrossing(current, v);
				this->removeCrossing(current, v->prev());
			}
		}

		current = current->next();
	} while (current != base);

	order();
}

}}
