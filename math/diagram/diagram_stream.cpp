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
		const std::vector<Vertex*> vertices(std::begin(list), std::end(list));
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
	std::map<Vertex*,std::size_t> nums;
	int index = 0;
	for (const auto &vertex : this->vertices()) {
		nums[vertex] = index;
    os << vertex->x() << " " << vertex->y() << "\n";
		index += 1;
	}

  {
    int cnum = 0;
		for (const auto &vertex : this->vertices()) {
			cnum += vertex->crossings().size();
		}

    os << "#CROSSINGS " << cnum << "\n";
  }

	for (const auto &vertex : this->vertices()) {
    for (const auto &crs : vertex->crossings()) {
      os << nums[vertex] << " " << nums[crs.up()] << "\n";
		}
  }
}

}}
