#include "knot.h"

const std::vector<double> KE::ThreeD::Knot::len_table() const {
	std::vector<double> table;
	table.reserve(this->points.size());
	for (std::size_t i = 0; i < this->points.size(); ++i) {
		table.push_back(Vector(this->points[this->next(i)], this->points[i]).length());
	}
	return table;
}
