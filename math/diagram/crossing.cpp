#include "diagram.h"

namespace KE { namespace TwoD {

std::shared_ptr<FloatPoint> Diagram::Crossing::coords() const {
	const int d0 = this->up().dy() * this->down().dx() - this->up().dx() * this->down().dy();

	if (d0 == 0) {
	  return nullptr;
	}
	
	const int d1 =
	  	(this->down().start->y() - this->up().start->y()) * this->down().dx()
		-	(this->down().start->x() - this->up().start->x()) * this->down().dy();

	return std::make_shared<FloatPoint>(
		this->up().start->x() + 1.0 * this->up().dx() * d1 / d0,
		this->up().start->y() + 1.0 * this->up().dy() * d1 / d0
	);
}

}}
