#include "knot.h"
#include "computables/length.h"

namespace KE { namespace ThreeD {

namespace {

std::mutex snapshotMutex;

}

Knot::Snapshot::Snapshot(const Knot &knot, const std::vector<Point> &points) : knot(knot), points(new std::vector<Point>(points)), _edgeLengths(new std::vector<double>), generation(knot.generation()) {
}

Knot::Snapshot Knot::points() const {
	if (this->lockCount > 0 || !this->latest || this->latest->generation != this->generation()) {
		std::lock_guard<std::recursive_mutex> guard(this->dataChangeMutex);
		this->latest = std::shared_ptr<Snapshot>(new Snapshot(*this, this->_points));
	}
	return *this->latest;
}

const std::vector<double> &Knot::Snapshot::edgeLengths() const {
	if (this->_edgeLengths->empty()) {
		std::lock_guard<std::mutex> guard(snapshotMutex);
		if (this->_edgeLengths->empty()) {
			this->_edgeLengths->reserve(this->size());
			for (std::size_t i = 1; i < this->size(); ++i) {
				this->_edgeLengths->push_back((*this)[i].distanceTo((*this)[i - 1]));
			}
			this->_edgeLengths->push_back((*this)[this->size() - 1].distanceTo((*this)[0]));
		}
	}
	return *this->_edgeLengths;
}

void Knot::create_depend() {
  this->length = std::make_shared<Computables::Length>(*this);
}

}}
