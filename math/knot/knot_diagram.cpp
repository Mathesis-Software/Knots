#include <cmath>

#include "knot.h"
#include "../diagram/diagram.h"

namespace KE { namespace ThreeD {

Knot::Knot(const TwoD::Diagram &diagram, int w, int h) : caption(diagram.caption + " (from diagram)") {
	const auto edges = diagram.edges();
	for (const auto &edge : edges) {
		const auto coords = edge.start->coords();
		points.push_back(Point(
			2.4 * coords.x / w - 1.2,
			1.2 - 2.4 * coords.y / h,
			0
		));
		
		for (const auto &crs : diagram.crossings(edge, true)) {
			std::shared_ptr<TwoD::FloatPoint> current = crs.coords();
			if (!current) {
				continue;
			}
			points.push_back(Point(
				2.4 * current->x / w - 1.2,
				1.2 - 2.4 * current->y / h,
				crs.up == edge ? 0.2 : -0.2
			));
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
