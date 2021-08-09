#include <math.h>

#include "knot.h"

namespace KE { namespace ThreeD {

const std::vector<double> KE::ThreeD::Knot::len_table() const {
	const auto points = this->points();
	std::vector<double> table;
	table.reserve(points.size());
	for (std::size_t i = 0; i < points.size() - 1; ++i) {
		table.push_back(points[i].distanceTo(points[i + 1]));
	}
	table.push_back(points[points.size() - 1].distanceTo(points[0]));
	return table;
}

}}
