#include "knot.h"

const std::vector<double> KE::ThreeD::Knot::len_table() const {
	const auto points = this->points();
	std::vector<double> table;
	table.reserve(points.size());
	for (std::size_t i = 0; i < points.size(); ++i) {
		table.push_back(Vector(points[points.next(i)], points[i]).length());
	}
	return table;
}
