#include "length.h"
#include "../knot.h"

namespace KE { namespace ThreeD { namespace Computables {

Length::Length(const Knot &knot) : Computable(knot, "Length") {
}

double Length::compute() {
	const auto points = this->knot.points();

	double value = points[0].distanceTo(points[points.size() - 1]);
	for (std::size_t i = 1; i < points.size(); ++i) {
		value += points[i - 1].distanceTo(points[i]);
	}
	return value;
}

}}}
