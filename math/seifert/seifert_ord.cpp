#include "seifert.h"

seifert_ord *seifert_ord::insert(seifert *s) {
  if (value->point.x > s->point.x) {
    if (!prev) {
      prev = new seifert_ord (s);
      prev->next = this;
      return prev;
    } else {
      return prev->insert (s);
		}
  } else if (!next) {
		next = new seifert_ord (s);
		next->prev = this;
		return next;
	} else if (next->value->point.x > s->point.x) {
		seifert_ord *so = new seifert_ord (s);
		so->next = next;
		so->prev = this;
		next->prev = so;
		next = so;
		return so;
	} else {
		return next->insert (s);
  }
}

seifert_ord::seifert_ord(seifert *s) {
  value = s;
  next = nullptr;
  prev = nullptr;
}

seifert_ord::~seifert_ord() {
  if (next)
    next->prev = nullptr;

  if (prev)
    prev->next = nullptr;
}
