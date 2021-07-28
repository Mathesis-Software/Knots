#include <cmath>

#include "knot.h"

void Knot::fill_len_table() {
  if (!len_table.empty()) {
    return;
	}

  for (std::size_t i = 0; i < this->points.size(); ++i) {
		const double dx = points[i].x - points[next(i)].x;
		const double dy = points[i].y - points[next(i)].y;
		const double dz = points[i].z - points[next(i)].z;
    this->len_table.push_back(sqrt(dx * dx + dy * dy + dz * dz));
  }
}
