#include <math.h>

#include "knot.h"

void knot::create_len_table() {
  if (len_table)
    return;

  len_table = new double [this->points.size()];

  for (std::size_t i = 0; i < this->points.size(); i++)
  {
    len_table [i] =
      sqrt (
        (points[i].x - points[next (i)].x) *
          (points[i].x - points[next (i)].x) +
        (points[i].y - points[next (i)].y) *
          (points[i].y - points[next (i)].y) +
        (points[i].z - points[next (i)].z) *
          (points[i].z - points[next (i)].z) );
  }
}

void knot::delete_len_table (void)
{
  if (len_table)
  {
    delete[] len_table;
    len_table = NULL;
  }
}
