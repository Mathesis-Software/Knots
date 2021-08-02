#include "diagram.h"

namespace KE { namespace TwoD {

Diagram::Vertex::Vertex(int x, int y) {
  vertex_prev = this;
  vertex_next = this;
  coord_x = x;
  coord_y = y;
}

Diagram::Vertex::Vertex(Vertex* v, int x, int y) {
  vertex_prev = v;
  vertex_next = v->vertex_next;
  vertex_prev->vertex_next = this;
  vertex_next->vertex_prev = this;
  coord_x = x;
  coord_y = y;
}

Diagram::Vertex::~Vertex() {
  vertex_prev->vertex_next = vertex_next;
  vertex_next->vertex_prev = vertex_prev;
}

void Diagram::Vertex::exclude() {
  vertex_prev->vertex_next = vertex_next;
  vertex_next->vertex_prev = vertex_prev;
}

void Diagram::Vertex::move(int x, int y) {
  coord_x += x;
  coord_y += y;
}

void Diagram::Vertex::moveTo(int x, int y) {
  coord_x = x;
  coord_y = y;
}

}}
