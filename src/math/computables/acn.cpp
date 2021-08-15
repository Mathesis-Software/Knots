#include "computables.h"
#include "../knotWrapper/KnotWrapper.h"

namespace KE { namespace ThreeD { namespace Computables {

AverageCrossingNumber::AverageCrossingNumber(const KnotWrapper &knot, bool withSign) :
	Computable(knot, withSign ? "Average signed crossing number" : "Average crossing number"),
	withSign(withSign) {
}

double AverageCrossingNumber::compute(const Knot::Snapshot &snapshot) {
	double value = 0.0;

	// tangent vectors
	std::vector<Vector> tangents;
	for (std::size_t i = 0; i < snapshot.size(); ++i) {
		tangents.push_back(Vector(snapshot[i], snapshot[snapshot.next(i)]));
	}

	for (std::size_t i = 0; i < snapshot.size(); ++i) {
		for (std::size_t j = 0; j < i; ++j) {
			const Vector chord(
				(snapshot[i].x + snapshot[snapshot.next(i)].x - snapshot[j].x - snapshot[snapshot.next(j)].x) / 2,
				(snapshot[i].y + snapshot[snapshot.next(i)].y - snapshot[j].y - snapshot[snapshot.next(j)].y) / 2,
				(snapshot[i].z + snapshot[snapshot.next(i)].z - snapshot[j].z - snapshot[snapshot.next(j)].z) / 2
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
