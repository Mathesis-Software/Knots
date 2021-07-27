#include <math.h>

#include "knot.h"
#include "../diagram/diagram.h"

knot::knot (diagram *D, int w, int h) : caption("Converted from diagram") {
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

  {
    vertex *v = D -> base;
    do {
			points.push_back(point(
				2.4 * v -> x () / w - 1.2,
				1.2 - 2.4 * v -> y () / h,
				0
			));
      
      crossing *crs = v -> crs ();
      while (crs) {
				points.push_back(point(
					2.4 * crs -> x () / w - 1.2,
					1.2 - 2.4 * crs -> y () / h,
					-0.4
				));
				if (crs -> next ()) {
					points.push_back(point(
						1.2 * (crs -> x () + crs -> next () -> x ()) / w - 1.2,
						1.2 - 1.2 * (crs -> y () + crs -> next () -> y () ) / h,
						0
					));
				}
        crs = crs -> next ();
      }
      v = v -> next ();
    }
    while (v != D -> base);
  }

  create_depend ();

  normalize (4 * length);
  center ();
}
