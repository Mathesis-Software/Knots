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
  vertex *v = base -> next ();
  int l;

  for (l = 1; v != base; l++)
    v = v -> next ();

  return l;
}
