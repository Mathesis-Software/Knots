#include "diagram.h"

namespace KE { namespace TwoD {

Diagram::Diagram() : caption("New Diagram"), base(nullptr) {
}

Diagram::~Diagram() {
	this->clear();
}

void Diagram::clear() {
	while (base != nullptr) {
		removeVertex(base);
	}
}

int Diagram::length() {
	vertex *v = base->next();
	int l;
	for (l = 1; v != base; l++) {
		v = v->next();
	}
	return l;
}

void Diagram::order() {
  vertex *v = base;
  do {
    v->order();
    v = v->next();
  } while (v != base);
}

void Diagram::shift(int x, int y) {
	vertex *v = base;
	do {
		v->move(x, y);
		v = v->next();
	} while (v != base);
}

}}
