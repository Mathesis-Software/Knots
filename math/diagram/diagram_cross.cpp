#include <stdlib.h>

#include "diagram.h"

bool diagram::tryAddCrossing (vertex *v1, vertex *v2)
{
  if (!crossed (v1, v2))
    return 0;

  new crossing (v1, v2);

  return 1;
}

bool diagram::tryChangeCrossing (vertex *v1, vertex *v2)
{
  if (tryRemoveCrossing (v2, v1))
    return tryAddCrossing (v1, v2);

  return 0;
}

bool diagram::tryRemoveCrossing (vertex *v1, vertex *v2)
{
  for (crossing *c = v1 -> crs (); c; c = c -> next ())
    if (c -> up () == v2)
    {
      delete c;
      return 1;
    }

  return 0;
}

bool diagram::isCrossing (vertex *v1, vertex *v2)
{
  for (crossing *c = v1 -> crs (); c; c = c -> next ())
    if (c -> up () == v2)
      return 1;

  return 0;
}
