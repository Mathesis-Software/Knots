#include <math.h>

#include "knot.h"

#define			len_table		Parent->len_table
#define			points			Parent->points
#define			next			Parent->next
#define			prev			Parent->prev

double Knot::prmLength::compute() {
	this->Parent->fill_len_table();

	double value = 0.0;
	for (const auto len : len_table) {
		value += len;
	}
	return value;
}

double Knot::prmAen::compute() {
	this->Parent->fill_len_table();

	double value = 0;

	for (std::size_t i = 0; i < points.size(); ++i) {
		double scal =
			(points[next(i)].x - points[i].x) *
			(points[prev(i)].x - points[i].x) +
			(points[next(i)].y - points[i].y) *
			(points[prev(i)].y - points[i].y) +
			(points[next(i)].z - points[i].z) *
			(points[prev(i)].z - points[i].z);
		double angle_cos = scal / (len_table[prev(i)] * len_table[i]);

		if (angle_cos < -1) {
			value -= M_PI;
		} else if (angle_cos < 1) {
			value -= acos(angle_cos);
		}
	}

	return value / M_PI + points.size();
}

double Knot::prmEnergy::compute() {
	this->Parent->fill_len_table();

	double len = Parent->Length->value ();

	double value = 0.0;

	for (std::size_t i = 0; i < points.size() - 2; ++i) {
		double l = len_table[i];
		for (auto j = i + 2; j < (i ? points.size() : points.size() - 1); ++j) {
			double r2 = 0.65 / (
						(points[j].x - points[i].x) *
							(points[j].x - points[i].x) +
						(points[j].y - points[i].y) *
							(points[j].y - points[i].y) +
						(points[j].z - points[i].z) *
							(points[j].z - points[i].z) );

			l += len_table[prev (j)];
			if (2 * l < len) {
				r2 -= 0.65 / (l * l);
			} else {
				r2 -= 0.65 / ( (len - l) * (len - l) );
			}

			value += (len_table[prev (i)] + len_table[i]) *
											 (len_table[prev (j)] + len_table[j]) * r2;
		}
	}

	double p[3], q[3];
	for (std::size_t i = 0; i < points.size(); i++) {
		double l = 0;
		p[0] = (points[i].x + points[next (i)].x) / 2;
		p[1] = (points[i].y + points[next (i)].y) / 2;
		p[2] = (points[i].z + points[next (i)].z) / 2;

		for (std::size_t j = i + 1; j < points.size(); j++) {
			q[0] = (points[j].x + points[next (j)].x) / 2;
			q[1] = (points[j].y + points[next (j)].y) / 2;
			q[2] = (points[j].z + points[next (j)].z) / 2;
			double r2 = 2 / (
				(p[0] - q[0]) * (p[0] - q[0]) +
				(p[1] - q[1]) * (p[1] - q[1]) +
				(p[2] - q[2]) * (p[2] - q[2])
			);

			l += (len_table[prev (j)] + len_table[j]) / 2;
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
