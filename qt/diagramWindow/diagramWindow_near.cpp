#include <math.h>
#include <stdlib.h>

#include "diagramWindow.h"

vertex *diagramWindow::nearToVertex (int x, int y)
{
  if (!base)
    return NULL;

  vertex *v = base;

  do
  {
    if ( (x - v -> x ()) * (x - v -> x ()) +
         (y - v -> y ()) * (y - v -> y ()) < 17 )
      return v;
    v = v -> next ();
  }
  while (v != base);

  return NULL;
}

crossing *diagramWindow::nearToCross (int x, int y)
{
  if (!base)
    return NULL;

  vertex *v = base;
  do
  {
    crossing *crs = v -> crs ();
    while (crs)
    {
      if ( (x - crs -> x ()) * (x - crs -> x ()) +
           (y - crs -> y ()) * (y - crs -> y ()) < 17 )
        return crs;

      crs = crs -> next ();
    }
    v = v -> next ();
  }
  while (v != base);

  return NULL;
}

vertex *diagramWindow::nearToEdge (int x, int y)
{
  if (!base)
    return NULL;

  vertex *v = base;
  do
  {
    int X = v -> next () -> x () - v -> x ();
    int Y = v -> next () -> y () - v -> y ();

    if ( ((x - v -> x ()) * X + (y - v -> y ()) * Y >= 0) &&
      ((x - v -> next () -> x ()) * X + (y - v -> next () -> y ()) * Y <= 0) &&
      (abs ( (x - v -> x ()) * Y - (y - v -> y ()) * X ) <= 5 * hypot (X, Y)) )
        return v;

    v = v -> next ();
  }
  while (isClosed ? (v != base) : (v -> next () != base));

  return NULL;
}
