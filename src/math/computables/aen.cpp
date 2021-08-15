#include "computables.h"
#include "../knotWrapper/KnotWrapper.h"

namespace KE { namespace ThreeD { namespace Computables {

AverageExtremumNumber::AverageExtremumNumber(const KnotWrapper &knot) :
	Computable(knot, "Average extremum number") {
}

double AverageExtremumNumber::compute(const Knot::Snapshot &snapshot) {
	const auto &edgeLengths = snapshot.edgeLengths();

	std::vector<Vector> edges;
	edges.reserve(snapshot.size());
	for (std::size_t i = 0; i < snapshot.size() - 1; ++i) {
		edges.push_back(Vector(snapshot[i], snapshot[i + 1]));
	}
	edges.push_back(Vector(snapshot[snapshot.size() - 1], snapshot[0]));

	double value = 0;

	for (std::size_t i = 0; i < snapshot.size(); ++i) {
		const auto &forward = edges[i];
		const auto &back = edges[snapshot.prev(i)];
		double angle_cos = - forward.scalar_product(back) / (edgeLengths[snapshot.prev(i)] * edgeLengths[i]);

		if (angle_cos < -1) {
			value -= M_PI;
		} else if (angle_cos < 1) {
			value -= acos(angle_cos);
		}
	}

	return value / M_PI + snapshot.size();
}

}}}
