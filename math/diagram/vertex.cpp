#include "diagram.h"

vertex::vertex(int x, int y) {
  vertex_prev = this;
  vertex_next = this;
  vertex_crs = nullptr;
  coord_x = x;
  coord_y = y;
}

vertex::vertex(vertex* v, int x, int y) {
  vertex_prev = v;
  vertex_next = v->vertex_next;
  vertex_prev->vertex_next = this;
  vertex_next->vertex_prev = this;
  vertex_crs = nullptr;
  coord_x = x;
  coord_y = y;
}

vertex::~vertex() {
  vertex_prev->vertex_next = vertex_next;
  vertex_next->vertex_prev = vertex_prev;

  while (vertex_crs) {
    delete vertex_crs;
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
  if (!crs() || !crs()->next())
    return;

  crossing *current = crs();
  while (current->next()->next())
    current = current->next();
  crossing *previous; 
  
  if (abs(x() - next()->x()) > abs(y() - next()->y())) {
    if (x() > next()->x()) {
      while (current) {
        previous = current->prev(); 
        while (current->next() && current->x() < current->next()->x()) {
          current->plus();
				}
				current = previous;
      }
		} else {
      while (current) {
        previous = current->prev(); 
        while (current->next() && current->x() > current->next()->x()) {
          current->plus();
				}
				current = previous;
      }
    }
  } else {
    if (y() > next()->y()) {
      while (current) {
        previous = current->prev(); 
        while (current->next() && current->y() < current->next()->y()) {
          current->plus();
				}
				current = previous;
      }
		} else {
      while (current) {
        previous = current->prev(); 
        while (current->next() && current->y() > current->next()->y()) {
          current->plus();
				}
				current = previous;
      }
		}
  }
}
