#include "knot.h"

const std::vector<double> &KE::ThreeD::Knot::len_table() const {
  if (this->_len_table.empty()) {
		for (std::size_t i = 0; i < this->points.size(); ++i) {
			this->_len_table.push_back(Vector(this->points[this->next(i)], this->points[i]).length());
		}
	}
	return this->_len_table;
}
