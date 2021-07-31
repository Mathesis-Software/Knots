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
		
		crossing *crs = v->crs();
		while (crs) {
			points.push_back(Point(
				2.4 * crs->x() / w - 1.2,
				1.2 - 2.4 * crs->y() / h,
				-0.4
			));
			if (crs->next()) {
				points.push_back(Point(
					1.2 * (crs->x() + crs->next()->x()) / w - 1.2,
					1.2 - 1.2 * (crs->y() + crs->next()->y()) / h,
					0
				));
			}
			crs = crs->next();
		}
	}

  create_depend();

  normalize(4 * this->points.size());
  center();
}

}}
