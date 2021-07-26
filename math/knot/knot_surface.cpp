#include <math.h>

#include "knot_surface.h"
#include "knot.h"

static double SIN[9] =
  {0.0, M_SQRT1_2, 1.0, M_SQRT1_2, 0.0, -M_SQRT1_2, -1.0, -M_SQRT1_2, 0.0 };
static double COS[9] =
  {1.0, M_SQRT1_2, 0.0, -M_SQRT1_2, -1.0, -M_SQRT1_2, 0.0, M_SQRT1_2, 1.0 };

static void norm (double *vector)
{
  double length = sqrt (
    vector [0] * vector [0] +
    vector [1] * vector [1] +
    vector [2] * vector [2] );
  
  vector [0] /= length;
  vector [1] /= length;
  vector [2] /= length;
}

#define			next		Parent -> next
#define			prev		Parent -> prev

knot_surface::knot_surface (knot *p, double t)
{
  Parent = p;
  thickness = t;
  stripped = 1;
  sides = surface::Front;
}

void knot_surface::calculate (void)
{
  int length = Parent -> length;
  double **points = Parent -> points;

  if (!length)
    return;
  
  destroy ();

  double **normal1, **normal2;
  int *shift;
  
  /*
  Create normal vector table
  */
  
  {
    double *v = new double [3];
  
    normal1 = new double* [length];
    normal2 = new double* [length];
  
    for (int i = 0; i < length; i++)
    {
      v [0] = points [next (i)] [0] - points [prev (i)] [0];
      v [1] = points [next (i)] [1] - points [prev (i)] [1];
      v [2] = points [next (i)] [2] - points [prev (i)] [2];
      norm (v);
  
      normal1 [i] = new double[3];
      normal2 [i] = new double[3];
  
      if (fabs (v [1]) < M_SQRT1_2)
      {
        normal1 [i] [0] = v [2];
        normal1 [i] [1] = 0.0;
        normal1 [i] [2] = - v [0];
    
        normal2 [i] [0] = - v [0] * v [1];
        normal2 [i] [1] = v [0] * v [0] + v [2] * v [2];
        normal2 [i] [2] = - v [1] * v [2];
      }
      else
      {
        normal1 [i] [0] = 0.0;
        normal1 [i] [1] = - v [2];
        normal1 [i] [2] = v [1];
    
        normal2 [i] [0] = v [1] * v [1] + v [2] * v [2];
        normal2 [i] [1] = - v [0] * v [1];
        normal2 [i] [2] = - v [0] * v [2];
      }
  
      norm (normal1 [i]);
      norm (normal2 [i]);
    }
  
    delete[] v;
  }
  
  /*
  Create shift table
  */
  
  {
    double a, b;
  
    shift = new int[length];
  
    for (int i = 0; i < length; i++)
    {
      a = normal1 [i] [0] * normal1 [next (i)] [0]
        + normal1 [i] [1] * normal1 [next (i)] [1]
        + normal1 [i] [2] * normal1 [next (i)] [2];
      b = normal1 [i] [0] * normal2 [next (i)] [0]
        + normal1 [i] [1] * normal2 [next (i)] [1]
        + normal1 [i] [2] * normal2 [next (i)] [2];
    
      if (a > 0)
        if (b > 0)
          shift [i] = (b > a) ? 7 : 0;
        else
          shift [i] = (- b > a) ? 2 : 1;
      else
        if (b > 0)
          shift [i] = (b > - a) ? 6 : 5;
        else
          shift [i] = (b > a) ? 4 : 3;
    }
  }
  
  /*
  Create surface
  */
  
  {
    int j, j1, j2;
    double n0, n1, n2;
  
    for (j = 0; j < 8; j++)
    {
      j1 = j;
      j2 = (j + 1) & 0x7;
  
      for (int i = 0; i < length; i++)
      {
        n0 = normal1 [i] [0] * SIN [j1] + normal2 [i] [0] * COS [j1];
        n1 = normal1 [i] [1] * SIN [j1] + normal2 [i] [1] * COS [j1];
        n2 = normal1 [i] [2] * SIN [j1] + normal2 [i] [2] * COS [j1];
    
        addpoint (points [i] [0] + thickness * n0,
	          points [i] [1] + thickness * n1,
		  points [i] [2] + thickness * n2,
		  n0, n1, n2);
    
        n0 = normal1 [i] [0] * SIN [j2] + normal2 [i] [0] * COS [j2];
        n1 = normal1 [i] [1] * SIN [j2] + normal2 [i] [1] * COS [j2];
        n2 = normal1 [i] [2] * SIN [j2] + normal2 [i] [2] * COS [j2];

        addpoint (points [i] [0] + thickness * n0,
	          points [i] [1] + thickness * n1,
		  points [i] [2] + thickness * n2,
		  n0, n1, n2);
      
        j1 += shift [i];
        j2 += shift [i];
        j1 &= 0x7;
        j2 &= 0x7;
      }
  
      n0 = normal1 [0] [0] * SIN [j1] + normal2 [0] [0] * COS [j1];
      n1 = normal1 [0] [1] * SIN [j1] + normal2 [0] [1] * COS [j1];
      n2 = normal1 [0] [2] * SIN [j1] + normal2 [0] [2] * COS [j1];
    
      addpoint (points [0] [0] + thickness * n0,
                points [0] [1] + thickness * n1,
	        points [0] [2] + thickness * n2,
		n0, n1, n2);
    
      n0 = normal1 [0] [0] * SIN [j2] + normal2 [0] [0] * COS [j2];
      n1 = normal1 [0] [1] * SIN [j2] + normal2 [0] [1] * COS [j2];
      n2 = normal1 [0] [2] * SIN [j2] + normal2 [0] [2] * COS [j2];
    
      addpoint (points [0] [0] + thickness * n0,
                points [0] [1] + thickness * n1,
	        points [0] [2] + thickness * n2,
		n0, n1, n2);
    }
  }

  {
    for (int i = 0; i < length; i++)
    {
      delete[] normal1 [i];
      delete[] normal2 [i];
    }
    delete[] normal1;
    delete[] normal2;
    delete[] shift;
  }
}
