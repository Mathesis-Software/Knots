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
