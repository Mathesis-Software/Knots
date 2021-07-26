#include "diagram.h"

void diagram::shift (int x, int y)
{
  vertex *v = base;
  do
  {
    v -> move (x, y);
    v = v -> next ();
  }
  while (v != base);
}
