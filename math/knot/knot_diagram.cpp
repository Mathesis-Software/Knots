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
		
		const crossing *previous = nullptr;
		for (const auto &crs : v->crossings()) {
			if (previous) {
				points.push_back(Point(
					1.2 * (previous->x() + crs.x()) / w - 1.2,
					1.2 - 1.2 * (previous->y() + crs.y()) / h,
					0
				));
			}
			points.push_back(Point(
				2.4 * crs.x() / w - 1.2,
				1.2 - 2.4 * crs.y() / h,
				-0.4
			));
			previous = &crs;
		}
	}

  create_depend();

  normalize(4 * this->points.size());
  center();
}

}}
