#include "computables.h"
#include "../Knot.h"

namespace KE { namespace ThreeD { namespace Computables {

AverageCrossingNumber::AverageCrossingNumber(const Knot &knot, bool withSign) :
	Computable(knot, withSign ? "Average signed crossing number" : "Average crossing number"),
	withSign(withSign) {
}

double AverageCrossingNumber::compute() {
	double value = 0.0;

	// tangent vectors
	const auto points = this->knot.points();
	std::vector<Vector> tangents;
	for (std::size_t i = 0; i < points.size(); ++i) {
		tangents.push_back(Vector(points[i], points[points.next(i)]));
	}

	for (std::size_t i = 0; i < points.size(); ++i) {
		for (std::size_t j = 0; j < i; ++j) {
			const Vector chord(
				(points[i].x + points[points.next(i)].x - points[j].x - points[points.next(j)].x) / 2,
				(points[i].y + points[points.next(i)].y - points[j].y - points[points.next(j)].y) / 2,
				(points[i].z + points[points.next(i)].z - points[j].z - points[points.next(j)].z) / 2
			);
			const double chord_len = chord.length();
			const double summand = tangents[i].scalar_product(tangents[j].vector_product(chord)) / (chord_len * chord_len * chord_len);
			if (this->withSign) {
				value += summand;
			} else {
				value += fabs(summand);
			}
		}
	}

	return value / (2 * M_PI);
}

}}}
