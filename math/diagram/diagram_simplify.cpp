#include "diagram.h"

bool diagram::simplify (int depth)
{
  if (depth < 1)
    return 0;

  vertex *v = base;
  bool result = 0;
  bool didSomethingInThisCycle;
  bool mustBeRemoved;

  do
  {
    didSomethingInThisCycle = 0;

    do
    {
      if (length () == 3)
        break;

      mustBeRemoved = 1;
      vertex *t = v;
      for (int i = 1; i < depth; i++)
        t = t -> prev ();
      for (int i = 0; i < 2 * depth; i++)
      {
	mustBeRemoved &= !t -> crs ();
        t = t -> next ();
      }
      if (mustBeRemoved)
      {
	result = didSomethingInThisCycle = 1;
        removeVertex (v -> next ());
      }
      else
        v = v -> next ();
    }
    while (v != base);
  }
  while (didSomethingInThisCycle);

  return result;
}
