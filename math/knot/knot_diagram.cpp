#include <math.h>

#include "knot.h"
#include "../diagram/diagram.h"

knot::knot (diagram *D, int w, int h)
{
  Caption = new char [256];
  strcpy (Caption, "Converted from diagram");

  length = 0;
  
  {
    vertex *v = D -> base;
    do
    {
      crossing *c = v -> crs ();
      if (c)
        while (c)
	{
	  c = c -> next ();
	  length +=2;
	}
      else
        length++;

      v = v -> next ();
    }
    while (v != D -> base);
  }

  points = new double* [length];
  
  {
    int c = 0;

    vertex *v = D -> base;
    do
    {
      points [c] = new double [3];
      points [c] [0] = 2.4 * v -> x () / w - 1.2;
      points [c] [1] = 1.2 - 2.4 * v -> y () / h;
      points [c] [2] = 0;
      c ++;
      
      crossing *crs = v -> crs ();
      while (crs)
      {
        points [c] = new double [3];
        points [c] [0] = 2.4 * crs -> x () / w - 1.2;
        points [c] [1] = 1.2 - 2.4 * crs -> y () / h;
        points [c] [2] = -0.4;
	if (crs -> next ())
        {
	  c ++;
          points [c] = new double [3];
          points [c] [0] = 1.2 * (crs -> x () + crs -> next () -> x ()) / w - 1.2;
          points [c] [1] = 1.2 - 1.2 * (crs -> y () + crs -> next () -> y () ) / h;
          points [c] [2] = 0;
	}
        crs = crs -> next ();
        c ++;
      }
      v = v -> next ();
    }
    while (v != D -> base);
  }

  create_depend ();

  normalize (4 * length);
  center ();
}
