#include "computables.h"
#include "../knot.h"

namespace KE { namespace ThreeD { namespace Computables {

MoebiusEnergy::MoebiusEnergy(const Knot &knot) : Computable(knot, "Moebius energy") {
}

double MoebiusEnergy::compute() {
	const auto points = this->knot.points();
	const auto &edgeLengths = points.edgeLengths();

	double len = knot.length->value ();

	double value = 0.0;

	for (std::size_t i = 0; i < points.size() - 2; ++i) {
		double l = edgeLengths[i];
		for (auto j = i + 2; j < (i ? points.size() : points.size() - 1); ++j) {
			const Vector chord(points[i], points[j]);
			double r2 = 0.65 / chord.square();

			l += edgeLengths[points.prev(j)];
			if (2 * l < len) {
				r2 -= 0.65 / (l * l);
			} else {
				r2 -= 0.65 / ((len - l) * (len - l));
			}

			value += (edgeLengths[points.prev(i)] + edgeLengths[i]) *
							 (edgeLengths[points.prev(j)] + edgeLengths[j]) * r2;
		}
	}

	std::vector<Point> middles;
	middles.reserve(points.size());
	for (std::size_t i = 0; i < points.size(); i++) {
		middles.push_back(Point(
			(points[i].x + points[points.next(i)].x) / 2,
			(points[i].y + points[points.next(i)].y) / 2,
			(points[i].z + points[points.next(i)].z) / 2
		));
	}

	for (std::size_t i = 0; i < points.size(); i++) {
		double l = 0;
		for (std::size_t j = i + 1; j < points.size(); j++) {
			const Vector r(middles[i], middles[j]);
			double r2 = 2 / r.square();

			l += (edgeLengths[points.prev(j)] + edgeLengths[j]) / 2;
			if (2 * l < len) {
				r2 -= 2 / (l * l);
			} else {
				r2 -= 2 / ( (len - l) * (len - l) );
			}

			value += edgeLengths[i] * edgeLengths[j] * r2;
		}
	}

	value /= 2.3;
	value -= 4;
	return value;
}

}}}
