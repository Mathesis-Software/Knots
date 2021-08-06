#include <iostream>
#include <cmath>

#include "knot.h"
#include "../diagram/diagram.h"

namespace KE { namespace ThreeD {

Knot::Knot(const TwoD::Diagram &diagram, int w, int h) : caption("Converted from diagram") {
	const auto edges = diagram.edges();
	auto count_vertex = 0;
	auto count_down = 0;
	auto count_up = 0;
	for (const auto &edge : edges) {
		const auto coords = edge.start->coords();
		points.push_back(Point(
			2.4 * coords.x / w - 1.2,
			1.2 - 2.4 * coords.y / h,
			0
		));
		count_vertex += 1;
		
		for (const auto &crs : diagram.crossings(edge, true)) {
			std::shared_ptr<TwoD::FloatPoint> current = crs.coords();
			if (!current) {
				continue;
			}
			points.push_back(Point(
				2.4 * current->x / w - 1.2,
				1.2 - 2.4 * current->y / h,
				crs.up == edge ? 0.4 : -0.4
			));
			if (crs.up == edge) {
				count_up += 1;
			} else {
				count_down += 1;
			}
		}
	}

  create_depend();

	double min = points.front().distanceTo(points.back());
	double total = min;
	for (std::size_t i = 1; i < points.size(); ++i) {
		const double dist = points[i - 1].distanceTo(points[i]);
		min = std::min(min, dist);
		total += dist;
	}
  normalize(std::max(5 * this->points.size(), 3 * (std::size_t)std::round(total / min)));
  center();
}

}}
