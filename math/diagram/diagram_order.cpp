#include "diagram.h"

void diagram::order (void)
{
  vertex *v = base;
  do
  {
    v -> order ();
    v = v -> next ();
  }
  while (v != base);
}
