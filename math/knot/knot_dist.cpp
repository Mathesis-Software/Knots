#include <math.h>

#include "knot.h"

void knot::create_len_table (void)
{
  if (len_table)
    return;

  len_table = new double [length];

  for (int i = 0; i < length; i++)
  {
    len_table [i] =
      sqrt (
        (points [i] [0] - points [next (i)] [0]) *
          (points [i] [0] - points [next (i)] [0]) +
        (points [i] [1] - points [next (i)] [1]) *
          (points [i] [1] - points [next (i)] [1]) +
        (points [i] [2] - points [next (i)] [2]) *
          (points [i] [2] - points [next (i)] [2]) );
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
