#include <math.h>

#include "knot.h"
#include "../diagram/diagram.h"

namespace KE { namespace ThreeD {

Knot::Knot(const TwoD::Diagram &diagram, int w, int h) : caption("Converted from diagram") {
	for (auto v : diagram.vertices()) {
		points.push_back(Point(
			2.4 * v->x() / w - 1.2,
			1.2 - 2.4 * v->y() / h,
			0
		));
		
		std::shared_ptr<TwoD::FloatPoint> previous;
		for (const auto &crs : v->crossings()) {
			std::shared_ptr<TwoD::FloatPoint> current = crs.coords();
			if (!current) {
				continue;
			}
			if (previous) {
				points.push_back(Point(
					1.2 * (previous->x + current->x) / w - 1.2,
					1.2 - 1.2 * (previous->y + current->y) / h,
					0
				));
			}
			points.push_back(Point(
				2.4 * current->x / w - 1.2,
				1.2 - 2.4 * current->y / h,
				-0.4
			));
			previous = current;
		}
	}

  create_depend();

  normalize(4 * this->points.size());
  center();
}

}}
