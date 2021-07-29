#include <cstring>

#include "diagram.h"

namespace KE { namespace TwoD {

Diagram::Diagram(std::istream &is) : base(nullptr) {
  char tmp[256];

  is.get(tmp, 32, ' ');
  if (strcmp(tmp, "#DIAGRAM") != 0)
    return;

  is.get(tmp[0]);
  is.get(tmp, 255);
  this->caption = tmp;

  {
    int length = -1;
    is.get(tmp, 32, ' ');
    is >> length;
    if (strcmp(tmp, "\n#POINTS") != 0)
      return;

    if (length <= 0)
      return;

    int x, y;
    for (int i = 0; i < length; i++) {
      is >> x >> y;
      if (!is.good()) {
				this->clear();
        return;
      }
      this->addVertex(nullptr, x, y);
    }
  }

  {
    int length = -1;
    is.get(tmp, 32, ' ');
    is >> length;
    if (length < 0 || strcmp(tmp, "\n#CROSSINGS") != 0) {
      this->clear();
      return;
    }

    int x, y;
    for (int i = 0; i < length; i++) {
      is >> x >> y;
      if (!is.good() || x < 0 || y < 0 || x >= this->length() || y >= this->length()) {
				this->clear();
        return;
      }
      if (x > y) {
        if (!this->tryChangeCrossing(this->vByNum(x), this->vByNum(y))) {
          this->clear();
          return;
        }
      } else {
        if (!this->isCrossing(this->vByNum(x), this->vByNum(y))) {
          this->clear();
          return;
        }
      }
    }
  }
}

void Diagram::save(std::ostream &os) {
  vertex *v = this->base;

  os << "#DIAGRAM " << this->caption << "\n#POINTS " << this->length() << "\n";
  do {
    os << v->x() << " " << v->y() << "\n";
    v = v->next();
  } while (v != this->base);

  crossing *c;

  {
    int cnum = 0;
    do {
      for (c = v->crs(); c; c = c->next())
        cnum++;
      v = v->next();
    } while (v != this->base);

    os << "#CROSSINGS " << cnum << "\n";
  }

  do {
    for (c = v->crs(); c; c = c->next())
      os << this->numByV(v) << " "
         << this->numByV(c->up()) << "\n";
    v = v->next();
  } while (v != this->base);
}

}}
