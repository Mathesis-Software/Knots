#include <cstring>
#include <map>
#include <vector>

#include "diagram.h"

namespace KE { namespace TwoD {

Diagram::Diagram(std::istream &is) : base(nullptr), isClosed(true) {
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

		const auto list = this->vertices();
		const std::vector<vertex*> vertices(std::begin(list), std::end(list));
    int x, y;
    for (int i = 0; i < length; i++) {
      is >> x >> y;
      if (!is.good() || x < 0 || y < 0 || x >= this->length() || y >= this->length()) {
				this->clear();
        return;
      }
      if (x > y) {
        if (!this->tryChangeCrossing(vertices[x], vertices[y])) {
          this->clear();
          return;
        }
      } else {
        if (!this->isCrossing(vertices[x], vertices[y])) {
          this->clear();
          return;
        }
      }
    }
  }
}

void Diagram::save(std::ostream &os) {
  os << "#DIAGRAM " << this->caption << "\n#POINTS " << this->length() << "\n";
	std::map<vertex*,std::size_t> nums;
	int index = 0;
	for (auto vertex : this->vertices()) {
		nums[vertex] = index;
    os << vertex->x() << " " << vertex->y() << "\n";
		index += 1;
	}

  crossing *c;

  {
    int cnum = 0;
		for (auto vertex : this->vertices()) {
      for (c = vertex->crs(); c; c = c->next())
        cnum++;
		}

    os << "#CROSSINGS " << cnum << "\n";
  }

	for (auto vertex : this->vertices()) {
    for (c = vertex->crs(); c; c = c->next()) {
      os << nums[vertex] << " " << nums[c->up()] << "\n";
		}
  }
}

}}
