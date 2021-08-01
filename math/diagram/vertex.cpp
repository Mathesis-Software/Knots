#include <algorithm>
#include <functional>

#include "diagram.h"

vertex::vertex(int x, int y) {
  vertex_prev = this;
  vertex_next = this;
  coord_x = x;
  coord_y = y;
}

vertex::vertex(vertex* v, int x, int y) {
  vertex_prev = v;
  vertex_next = v->vertex_next;
  vertex_prev->vertex_next = this;
  vertex_next->vertex_prev = this;
  coord_x = x;
  coord_y = y;
}

vertex::~vertex() {
  vertex_prev->vertex_next = vertex_next;
  vertex_next->vertex_prev = vertex_prev;

	for (auto crs : this->_crossings) {
		delete crs;
	}
}

void vertex::exclude() {
  vertex_prev->vertex_next = vertex_next;
  vertex_next->vertex_prev = vertex_prev;
}

void vertex::move(int x, int y) {
  coord_x += x;
  coord_y += y;
}

void vertex::moveTo(int x, int y) {
  coord_x = x;
  coord_y = y;
}

void vertex::order() {
	std::function<bool(crossing*,crossing*)> comparator;
  if (abs(x() - next()->x()) > abs(y() - next()->y())) {
    if (x() > next()->x()) {
			comparator = [](crossing *c0, crossing *c1) { return c0->x() > c1->x(); };
		} else {
			comparator = [](crossing *c0, crossing *c1) { return c0->x() < c1->x(); };
		}
	} else {
    if (y() > next()->y()) {
			comparator = [](crossing *c0, crossing *c1) { return c0->y() > c1->y(); };
		} else {
			comparator = [](crossing *c0, crossing *c1) { return c0->y() < c1->y(); };
		}
	}
	this->_crossings.sort(comparator);
}
