#include <numeric>

#include "length.h"
#include "../Knot.h"

namespace KE { namespace ThreeD { namespace Computables {

Length::Length(const Knot &knot) : Computable(knot, "Length") {
}

double Length::compute() {
	const auto points = this->knot.points();
	const auto &edgeLengths = points.edgeLengths();
	return std::accumulate(edgeLengths.begin(), edgeLengths.end(), 0.0);
}

}}}
