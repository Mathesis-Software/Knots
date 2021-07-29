#include "diagram.h"

void diagram::order() {
  vertex *v = base;
  do {
    v->order();
    v = v->next();
  } while (v != base);
}
