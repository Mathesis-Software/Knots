#include <math.h>

#include "knot.h"

#define		NEXT(num)		(num == old_length - 1) ? 0 : num + 1

void knot::normalize(int num) {
  create_len_table ();

  double len = Length -> value ();

  int old_length = length;
  length = num;

  const auto old_points = points;
  points.clear();
  
  len /= length;

  {
    int v = 0;
    double part;
    double llen = len_table [0];
    double rest = llen;

    for (std::size_t i = 0; i < length; i++) {
      part = (1 - rest / llen); 
			points.push_back(point(
				old_points[v].x + part * ( old_points[NEXT (v)].x - old_points[v].x ),
				old_points[v].y + part * ( old_points[NEXT (v)].y - old_points[v].y ),
				old_points[v].z + part * ( old_points[NEXT (v)].z - old_points[v].z )
			));

      rest -= len;
      while (rest < 0)
      {
        v = NEXT (v);
        llen = len_table [v];
        rest += llen;
      }
    }
  }

  clear_depend ();
}
