#include "diagram.h"

bool diagram::simplify(int depth) {
  if (depth < 1)
    return 0;

  vertex *v = base;

  bool changed = false;
	for (bool continueFlag = true; continueFlag; ) {
    continueFlag = false;

    do {
      if (length() == 3)
        break;

      bool removeVertexFlag = true;
      vertex *t = v;
      for (int i = 1; i < depth; i++)
        t = t->prev();
      for (int i = 0; i < 2 * depth; i++) {
				removeVertexFlag &= !t->crs();
        t = t->next();
      }
      if (removeVertexFlag) {
				continueFlag = true;
				changed = true;
        removeVertex(v->next());
      } else {
        v = v->next();
			}
    } while (v != base);
  }

  return changed;
}
