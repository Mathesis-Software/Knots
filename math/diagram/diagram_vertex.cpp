#include "diagram.h"

namespace KE { namespace TwoD {

Diagram::Vertex *Diagram::addVertex(int x, int y) {
	if (this->isClosed()) {
		return nullptr;
	}

	if (!base) {
		base = new Vertex(x, y);
		return base;
	} else {
		Vertex *end = base->prev();
		Vertex *new_vertex = new Vertex(end, x, y);
		const Edge new_edge(end, new_vertex);
		for (const Edge &e : this->edges()) {
			if (e.intersects(new_edge)) {
				this->addCrossing(new_edge, e);
				e.orderCrossings();
			}
		}
		return new_vertex;
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

	std::shared_ptr<const Edge> removed1;
	std::shared_ptr<const Edge> removed2;
	for (const Edge &edge : this->edges()) {
		if (edge.end == v) {
			removed1 = std::make_shared<const Edge>(edge);
		} else if (edge.start == v) {
			removed2 = std::make_shared<const Edge>(edge);
		}
	}
	std::shared_ptr<const Edge> merged;
	if (removed1 && removed2) {
		merged = std::make_shared<const Edge>(removed1->start, removed2->end);
	}

	v->exclude();

	for (const Edge &edge : this->edges()) {
		std::shared_ptr<Crossing> removed_crossing1;
		if (removed1) {
			removed_crossing1 = this->getCrossing(*removed1, edge);
			this->removeCrossing(*removed1, edge);
		}
		std::shared_ptr<Crossing> removed_crossing2;
		if (removed2) {
			removed_crossing2 = this->getCrossing(*removed2, edge);
			this->removeCrossing(*removed2, edge);
		}

		if (!merged || !edge.intersects(*merged)) {
			continue;
		}
		if ((removed_crossing1 && removed_crossing1->up == edge) ||
				(removed_crossing2 && removed_crossing2->up == edge)) {
			this->addCrossing(edge, *merged);
		} else {
			this->addCrossing(*merged, edge);
		}
	}

	delete v;
}

void Diagram::moveVertex(Vertex *v, int x, int y) {
	if (v == nullptr)
		return;

	v->moveTo(x, y);

	std::shared_ptr<const Edge> changed1;
	std::shared_ptr<const Edge> changed2;
	for (const Edge &edge : this->edges()) {
		if (edge.end == v) {
			changed1 = std::make_shared<const Edge>(edge);
		} else if (edge.start == v) {
			changed2 = std::make_shared<const Edge>(edge);
		}
	}

	for (const Edge &edge : this->edges()) {
		auto changed_crossing1 = changed1 ? this->getCrossing(*changed1, edge) : nullptr;
		auto changed_crossing2 = changed2 ? this->getCrossing(*changed2, edge) : nullptr;

		if (changed1) {
			if (edge.intersects(*changed1)) {
				if (!changed_crossing1) {
					if (changed_crossing2 && changed_crossing2->up == edge) {
						this->addCrossing(edge, *changed1);
					} else {
						this->addCrossing(*changed1, edge);
					}
				}
			} else {
				this->removeCrossing(edge, *changed1);
			}
		}
		if (changed2) {
			if (edge.intersects(*changed2)) {
				if (!changed_crossing2) {
					if (changed_crossing1 && changed_crossing1->up == edge) {
						this->addCrossing(edge, *changed2);
					} else {
						this->addCrossing(*changed2, edge);
					}
				}
			} else {
				this->removeCrossing(edge, *changed2);
			}
		}
	}

	order();
}

void Diagram::close() {
	if (this->_isClosed) {
		return;
	}

	const auto edges = this->edges();
	if (edges.size() < 2) {
		return;
	}

	const Edge new_edge(edges.back().end, edges.front().start);
	for (const Edge &edge : edges) {
		if (new_edge.intersects(edge)) {
			this->addCrossing(new_edge, edge);
		}
	}
	this->_isClosed = true;
}

}}
