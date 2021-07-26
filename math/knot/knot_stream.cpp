#include "knot.h"

std::istream & operator >> (std::istream &is, knot *K)
{
  char tmp[32];

  is.get (tmp, 32, ' ');
  if (strcmp(tmp, "#KNOT") != 0)
    return is;

  is.get (tmp [0]);
  is.get (K -> Caption, 255);

  is.get (tmp, 32, ' ');
  is >> K -> length;
  if (strcmp(tmp, "\n#LENGTH") != 0)
    return is;

  if (K -> length <= 0)
  {
    K -> length = 0;
    return is;
  }

  K -> points = new double* [K -> length];

  for (int i = 0; i < K -> length; i++)
  {
    K -> points [i] = new double [3];
    is >> K -> points [i] [0] >> K -> points [i] [1] >> K -> points [i] [2];
    if (is.good ())
      continue;
    if (is.eof () && i == K -> length - 1)
      break;
    for (int j = 0; j <= i; j++)
      delete[] K -> points [i];
    delete[] K -> points;
    K -> points = NULL;
    K -> length = 0;
    break;
  }

  if (K -> length)
    K -> center ();

  return is;
}

std::ostream & operator << (std::ostream &os, knot *K)
{
  os << "#KNOT " << K -> Caption << "\n#LENGTH " << K -> length << "\n";

  for (int i = 0; i < K -> length; i++)
    os << K -> points [i] [0] << ' ' <<
          K -> points [i] [1] << ' ' <<
 	  K -> points [i] [2] << '\n';

  return os;
}
