#include "seifert.h"

seifert_ord *seifert_ord::insert (seifert *s)
{
  if (value -> coord [0] > s -> coord [0])
  {
    if (!prev)
    {
      prev = new seifert_ord (s);
      prev -> next = this;
      return prev;
    }
    else
      return prev -> insert (s);
  }
  else
  {
    if (!next)
    {
      next = new seifert_ord (s);
      next -> prev = this;
      return next;
    }
    else
      if (next -> value -> coord [0] > s -> coord [0])
      {
        seifert_ord *so = new seifert_ord (s);
	so -> next = next;
	so -> prev = this;
	next -> prev = so;
	next = so;
	return so;
      }
      else
        return next -> insert (s);
  }
}

seifert_ord::seifert_ord (seifert *s)
{
  value = s;
  next = NULL;
  prev = NULL;
}

seifert_ord::~seifert_ord (void)
{
  if (next)
    next -> prev = NULL;

  if (prev)
    prev -> next = NULL;
}
