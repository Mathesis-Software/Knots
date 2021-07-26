#include <math.h>

#include "knot.h"

#define		NEXT(num)		(num == old_length - 1) ? 0 : num + 1

void knot::normalize (int num)
{
  create_len_table ();

  double len = Length -> value ();

  int old_length = length;
  length = num;

  double **old_points = points;
  points = new double* [length];
  
  len /= length;

  {
    int v = 0;
    double part;
    double llen = len_table [0];
    double rest = llen;

    for (int i = 0; i < length; i++)
    {
      points [i] = new double [3];

      part = (1 - rest / llen); 
      points [i] [0] = old_points [v] [0]
            + part * ( old_points [NEXT (v)] [0] - old_points [v] [0] );
      points [i] [1] = old_points [v] [1]
            + part * ( old_points [NEXT (v)] [1] - old_points [v] [1] );
      points [i] [2] = old_points [v] [2]
            + part * ( old_points [NEXT (v)] [2] - old_points [v] [2] );

      rest -= len;
      while (rest < 0)
      {
        v = NEXT (v);
        llen = len_table [v];
        rest += llen;
      }
    }
  }

  {
    for (int i = 0; i < old_length; i++)
      delete[] old_points [i];
    delete[] old_points;
  }

  clear_depend ();
}
