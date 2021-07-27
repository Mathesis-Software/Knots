#include "diagram.h"

std::ostream & operator << (std::ostream & os, diagram *D)
{
  vertex *v = D -> base;

  os << "#DIAGRAM " << D -> caption << "\n#POINTS " << D -> length () << "\n";
  do
  {
    os << v -> x () << " " << v -> y () << "\n";
    v = v -> next ();
  }
  while (v != D -> base);

  crossing *c;

  {
    int cnum = 0;
    do
    {
      for (c = v -> crs (); c; c = c -> next ())
        cnum++;
      v = v -> next ();
    }
    while (v != D -> base);

    os << "#CROSSINGS " << cnum << "\n";
  }

  do
  {
    for (c = v -> crs (); c; c = c -> next ())
      os << D -> numByV (v) << " "
         << D -> numByV (c -> up ()) << "\n";
    v = v -> next ();
  }
  while (v != D -> base);

  return os;
}

std::istream & operator >> (std::istream & is, diagram *D)
{
  char tmp[256];

  D -> clear ();

  is.get(tmp, 32, ' ');
  if (strcmp (tmp, "#DIAGRAM") != 0)
    return is;

  is.get (tmp [0]);
  is.get (tmp, 255);
  D->caption = tmp;

  {
    int length = -1;
    is.get(tmp,32,' ');
    is >> length;
    if (strcmp(tmp, "\n#POINTS") != 0)
      return is;

    if (length <= 0)
      return is;

    int x, y;
    for (int i = 0; i < length; i++)
    {
      is >> x >> y;
      if (!is.good ())
      {
	D -> clear ();
        return is;
      }
      D -> addVertex (NULL, x, y);
    }
  }

  {
    int length = -1;
    is.get(tmp,32,' ');
    is >> length;
    if ( (length < 0) || (strcmp(tmp, "\n#CROSSINGS") != 0) )
    {
      D -> clear ();
      return is;
    }

    int x, y;
    for (int i = 0; i < length; i++)
    {
      is >> x >> y;
      if ( !is.good ()
           || (x < 0)
	   || (y < 0)
	   || (x >= D -> length ())
	   || (y >= D -> length ()) )
      {
	D -> clear ();
        return is;
      }
      if (x > y)
      {
        if (!D -> tryChangeCrossing (D -> vByNum (x), D -> vByNum (y)))
        {
          D -> clear ();
          return is;
        }
      }
      else
      {
        if (!D -> isCrossing (D -> vByNum (x), D -> vByNum (y)))
	{
          D -> clear ();
          return is;
        }
      }
    }
  }

  return is;
}
