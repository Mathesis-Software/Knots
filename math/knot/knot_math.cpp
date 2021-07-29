#include <math.h>

#include "knot.h"

namespace KE { namespace ThreeD {

double Knot::prmLength::compute() {
	double value = 0.0;
	for (const auto len : this->knot.len_table()) {
		value += len;
	}
	return value;
}

double Knot::prmAen::compute() {
	const auto &points = this->knot.points;
	const auto &len_table = this->knot.len_table();

	double value = 0;

	for (std::size_t i = 0; i < points.size(); ++i) {
		const Vector forward(points[i], points[this->knot.next(i)]);
		const Vector back(points[i], points[this->knot.prev(i)]);
		double angle_cos = forward.scalar_product(back) / (len_table[this->knot.prev(i)] * len_table[i]);

		if (angle_cos < -1) {
			value -= M_PI;
		} else if (angle_cos < 1) {
			value -= acos(angle_cos);
		}
	}

	return value / M_PI + points.size();
}

double Knot::prmEnergy::compute() {
	const auto &points = this->knot.points;
	const auto &len_table = this->knot.len_table();

	double len = knot.length->value ();

	double value = 0.0;

	for (std::size_t i = 0; i < points.size() - 2; ++i) {
		double l = len_table[i];
		for (auto j = i + 2; j < (i ? points.size() : points.size() - 1); ++j) {
			const Vector chord(points[i], points[j]);
			double r2 = 0.65 / chord.square();

			l += len_table[this->knot.prev(j)];
			if (2 * l < len) {
				r2 -= 0.65 / (l * l);
			} else {
				r2 -= 0.65 / ((len - l) * (len - l));
			}

			value += (len_table[this->knot.prev(i)] + len_table[i]) *
							 (len_table[this->knot.prev(j)] + len_table[j]) * r2;
		}
	}

	double p[3], q[3];
	for (std::size_t i = 0; i < points.size(); i++) {
		double l = 0;
		p[0] = (points[i].x + points[this->knot.next(i)].x) / 2;
		p[1] = (points[i].y + points[this->knot.next(i)].y) / 2;
		p[2] = (points[i].z + points[this->knot.next(i)].z) / 2;

		for (std::size_t j = i + 1; j < points.size(); j++) {
			q[0] = (points[j].x + points[this->knot.next(j)].x) / 2;
			q[1] = (points[j].y + points[this->knot.next(j)].y) / 2;
			q[2] = (points[j].z + points[this->knot.next(j)].z) / 2;
			double r2 = 2 / (
				(p[0] - q[0]) * (p[0] - q[0]) +
				(p[1] - q[1]) * (p[1] - q[1]) +
				(p[2] - q[2]) * (p[2] - q[2])
			);

			l += (len_table[this->knot.prev(j)] + len_table[j]) / 2;
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

}}
