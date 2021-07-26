#include "diagram.h"

diagram::diagram (void)
{
  Caption = new char [256];
  strcpy (Caption, "New Diagram");

  base = NULL;
}

diagram::~diagram (void)
{
  delete[] Caption;
  clear ();
}

void diagram::clear (void)
{
  while (base)
    removeVertex (base);
}

int diagram::length (void)
{
  vertex *v = base -> next ();
  int l;

  for (l = 1; v != base; l++)
    v = v -> next ();

  return l;
}
