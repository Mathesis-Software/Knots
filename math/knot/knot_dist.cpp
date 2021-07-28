#include <cmath>

#include "knot.h"

const std::vector<double> &KE::ThreeD::Knot::len_table() const {
  if (this->_len_table.empty()) {
		for (std::size_t i = 0; i < this->points.size(); ++i) {
			const double dx = points[i].x - points[next(i)].x;
			const double dy = points[i].y - points[next(i)].y;
			const double dz = points[i].z - points[next(i)].z;
			this->_len_table.push_back(sqrt(dx * dx + dy * dy + dz * dz));
		}
	}
	return this->_len_table;
}
