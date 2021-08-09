#include "computables.h"
#include "../knot.h"

namespace KE { namespace ThreeD { namespace Computables {

MoebiusEnergy::MoebiusEnergy(const Knot &knot) : Computable(knot, "Moebius energy") {
}

double MoebiusEnergy::compute() {
	const auto points = this->knot.points();
	const auto len_table = this->knot.len_table();

	double len = knot.length->value ();

	double value = 0.0;

	for (std::size_t i = 0; i < points.size() - 2; ++i) {
		double l = len_table[i];
		for (auto j = i + 2; j < (i ? points.size() : points.size() - 1); ++j) {
			const Vector chord(points[i], points[j]);
			double r2 = 0.65 / chord.square();

			l += len_table[points.prev(j)];
			if (2 * l < len) {
				r2 -= 0.65 / (l * l);
			} else {
				r2 -= 0.65 / ((len - l) * (len - l));
			}

			value += (len_table[points.prev(i)] + len_table[i]) *
							 (len_table[points.prev(j)] + len_table[j]) * r2;
		}
	}

	double p[3], q[3];
	for (std::size_t i = 0; i < points.size(); i++) {
		double l = 0;
		p[0] = (points[i].x + points[points.next(i)].x) / 2;
		p[1] = (points[i].y + points[points.next(i)].y) / 2;
		p[2] = (points[i].z + points[points.next(i)].z) / 2;

		for (std::size_t j = i + 1; j < points.size(); j++) {
			q[0] = (points[j].x + points[points.next(j)].x) / 2;
			q[1] = (points[j].y + points[points.next(j)].y) / 2;
			q[2] = (points[j].z + points[points.next(j)].z) / 2;
			double r2 = 2 / (
				(p[0] - q[0]) * (p[0] - q[0]) +
				(p[1] - q[1]) * (p[1] - q[1]) +
				(p[2] - q[2]) * (p[2] - q[2])
			);

			l += (len_table[points.prev(j)] + len_table[j]) / 2;
			if (2 * l < len) {
				r2 -= 2 / (l * l);
			} else {
				r2 -= 2 / ( (len - l) * (len - l) );
			}

			value += len_table[i] * len_table[j] * r2;
		}
	}

	value /= 2.3;
	value -= 4;
	return value;
}

}}}
