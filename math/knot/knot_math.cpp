#include <math.h>

#include "knot.h"

#define			create_len_table	Parent -> create_len_table
#define			len_table		Parent -> len_table
#define			length			Parent -> length
#define			points			Parent -> points
#define			next			Parent -> next
#define			prev			Parent -> prev

void knot::prmLength::compute (void)
{
  create_len_table ();

  internalValue = 0.0;

  for (int i = 0; i < length; i++)
    internalValue += len_table [i];
}

void knot::prmAen::compute (void)
{
  create_len_table ();

  internalValue = 0;

  double scal, angle_cos;
  
  for (int i = 0; i < length; i++)
  {
    scal = (points [next (i)] [0] - points [i] [0]) *
           (points [prev (i)] [0] - points [i] [0]) +
           (points [next (i)] [1] - points [i] [1]) *
           (points [prev (i)] [1] - points [i] [1]) +
           (points [next (i)] [2] - points [i] [2]) *
           (points [prev (i)] [2] - points [i] [2]);
    angle_cos = scal / (len_table [prev (i)] * len_table [i]);

    if (angle_cos < -1) 
      internalValue -= M_PI; 
    else
      if (angle_cos < 1)
        internalValue -= acos (angle_cos); 
  }

  internalValue /= M_PI;
  internalValue += length;
}

void knot::prmEnergy::compute (void)
{
  create_len_table ();

  double r2, l;
  double len = Parent -> Length -> value ();

  internalValue = 0.0;

  for (int i = 0; i < length - 2; i++)
  {
    l = len_table [i];
    for (int j = i + 2; j < (i ? length : length - 1); j++)
    {
      r2 = 0.65 / (
            (points [j] [0] - points [i] [0]) *
              (points [j] [0] - points [i] [0]) +
            (points [j] [1] - points [i] [1]) *
              (points [j] [1] - points [i] [1]) +
            (points [j] [2] - points [i] [2]) *
              (points [j] [2] - points [i] [2]) );

      l += len_table [prev (j)];
      if (2 * l < len)
        r2 -= 0.65 / (l * l);
      else
        r2 -= 0.65 / ( (len - l) * (len - l) );

      internalValue += (len_table [prev (i)] + len_table [i]) *
                       (len_table [prev (j)] + len_table [j]) * r2;
    }
  }

  double p[3], q[3];
  for (int i = 0; i < length; i++)
  {
    l = 0;
    p[0] = (points [i] [0] + points [next (i)] [0]) / 2;
    p[1] = (points [i] [1] + points [next (i)] [1]) / 2;
    p[2] = (points [i] [2] + points [next (i)] [2]) / 2;
  
    for (int j = i + 1; j < length; j++)
    {
      q[0] = (points [j] [0] + points [next (j)] [0]) / 2;
      q[1] = (points [j] [1] + points [next (j)] [1]) / 2;
      q[2] = (points [j] [2] + points [next (j)] [2]) / 2;
      r2 = 2 / (
            (p [0] - q [0]) * (p [0] - q [0]) +
            (p [1] - q [1]) * (p [1] - q [1]) +
            (p [2] - q [2]) * (p [2] - q [2]) );
  
      l += (len_table [prev (j)] + len_table [j]) / 2;
      if (2 * l < len)
        r2 -= 2 / (l * l);
      else
        r2 -= 2 / ( (len - l) * (len - l) );
  
      internalValue += len_table [i] * len_table [j] * r2;
    }
  }

  internalValue /= 2.3;
  internalValue -= 4;
}
