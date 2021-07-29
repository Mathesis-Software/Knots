#include <math.h>

#include "knot.h"
#include "../diagram/diagram.h"

namespace KE { namespace ThreeD {

Knot::Knot(TwoD::Diagram *D, int w, int h) : caption("Converted from diagram") {
  vertex *v = D->base;
	do {
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
		v = v->next();
	} while (v != D->base);

  create_depend();

  normalize(4 * this->points.size());
  center();
}

}}
