#include <stdlib.h>
#include "diagram.h"

crossing::crossing(vertex* v1, vertex *v2) {
  arc_up = v2;
  arc_down = v1;
	v1->_crossings.push_back(this);
	v1->order();
}

float crossing::x() {
  int d0, d1;

  d0 = (up()->next()->x() - up()->x())
     * (down()->y() - down()->next()->y())
     - (up()->next()->y() - up()->y())
     * (down()->x() - down()->next()->x());

  if (d0 == 0)
    return 0.0;
  
  d1 = (down()->x() - up()->x())
     * (down()->y() - down()->next()->y())
     - (down()->y() - up()->y())
     * (down()->x() - down()->next()->x());

  return up()->x() + 1.0 * (up()->next()->x() - up()->x()) * d1 / d0;
}     

float crossing::y() {
  int d0, d1;

  d0 = (up()->next()->x() - up()->x()) * (down()->y() - down()->next()->y())
     - (up()->next()->y() - up()->y()) * (down()->x() - down()->next()->x());

  if (d0 == 0)
    return 0.0;
  
  d1 = (down()->x() - up()->x())
     * (down()->y() - down()->next()->y())
     - (down()->y() - up()->y())
     * (down()->x() - down()->next()->x());

  return up()->y() + 1.0 * (up()->next()->y() - up()->y()) * d1 / d0;
}
