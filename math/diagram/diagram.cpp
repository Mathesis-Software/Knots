#include "diagram.h"

diagram::diagram() : caption("New Diagram"), base(nullptr) {
}

diagram::~diagram() {
	this->clear();
}

void diagram::clear() {
	while (base != nullptr) {
		removeVertex(base);
	}
}

int diagram::length() {
	vertex *v = base->next();
	int l;
	for (l = 1; v != base; l++) {
		v = v->next();
	}
	return l;
}

void diagram::order() {
  vertex *v = base;
  do {
    v->order();
    v = v->next();
  } while (v != base);
}

void diagram::shift(int x, int y) {
	vertex *v = base;
	do {
		v->move(x, y);
		v = v->next();
	} while (v != base);
}
