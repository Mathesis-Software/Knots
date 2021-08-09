#include "computables.h"
#include "../knot.h"

namespace KE { namespace ThreeD { namespace Computables {

AverageExtremumNumber::AverageExtremumNumber(const Knot &knot) :
	Computable(knot, "Average extremum number") {
}

double AverageExtremumNumber::compute() {
	const auto points = this->knot.points();

	std::vector<Vector> edges;
	edges.reserve(points.size());
	for (std::size_t i = 0; i < points.size() - 1; ++i) {
		edges.push_back(Vector(points[i], points[i + 1]));
	}
	edges.push_back(Vector(points[points.size() - 1], points[0]));

	std::vector<double> lengths;
	lengths.reserve(edges.size());
	for (const auto &edge : edges) {
		lengths.push_back(edge.length());
	}

	double value = 0;

	for (std::size_t i = 0; i < points.size(); ++i) {
		const auto &forward = edges[i];
		const auto &back = edges[points.prev(i)];
		double angle_cos = - forward.scalar_product(back) / (lengths[points.prev(i)] * lengths[i]);

		if (angle_cos < -1) {
			value -= M_PI;
		} else if (angle_cos < 1) {
			value -= acos(angle_cos);
		}
	}

	return value / M_PI + points.size();
}

}}}
