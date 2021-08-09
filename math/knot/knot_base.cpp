#include "knot.h"
#include "computables/length.h"

namespace KE { namespace ThreeD {

Knot::Snapshot::Snapshot(const Knot &knot, const std::vector<Point> &points) : knot(knot), points(new std::vector<Point>(points)), generation(knot.generation()) {
}

Knot::Snapshot Knot::points() const {
	if (this->lockCount > 0 || !this->latest || this->latest->generation != this->generation()) {
		std::lock_guard<std::recursive_mutex> guard(this->dataChangeMutex);
		this->latest = std::shared_ptr<Snapshot>(new Snapshot(*this, this->_points));
	}
	return *this->latest;
}

void Knot::create_depend() {
  this->length = std::make_shared<Computables::Length>(*this);
}

}}
