#include <math.h>

#include "knot.h"

namespace KE { namespace ThreeD {

void Knot::normalize(std::size_t num) {
	const auto len_table = this->len_table();

	double len = this->length->value();

	const auto old_points = points;
	points.clear();

	len /= num;

	{
		std::size_t v = 0;
		double part;
		double llen = len_table[0];
		double rest = llen;

		for (std::size_t i = 0; i < num; i++) {
			const auto next_v = v == old_points.size() - 1 ? 0 : v + 1;

			part = (1 - rest / llen);
			points.push_back(Point(
				old_points[v].x + part * (old_points[next_v].x - old_points[v].x),
				old_points[v].y + part * (old_points[next_v].y - old_points[v].y),
				old_points[v].z + part * (old_points[next_v].z - old_points[v].z)
			));

			rest -= len;
			while (rest < 0) {
				v = v == old_points.size() - 1 ? 0 : v + 1;
				llen = len_table[v];
				rest += llen;
			}
		}
	}

	clear_depend();
}

}}
