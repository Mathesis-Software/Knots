#include <numeric>

#include "knot.h"

namespace KE { namespace ThreeD {

std::vector<Point> Knot::normalizedPoints(const Snapshot &snapshot, std::size_t newNumberOfPoints) const {
	const auto &edgeLengths = snapshot.edgeLengths();
	const double totalLength = std::accumulate(edgeLengths.begin(), edgeLengths.end(), 0.0);
	const double newEdgeLength = totalLength / newNumberOfPoints;

	std::vector<Point> newPoints;
	newPoints.reserve(newNumberOfPoints);

	std::size_t v = 0;
	double llen = edgeLengths[0];
	double rest = llen;

	for (std::size_t i = 0; i < newNumberOfPoints; i++) {
		const auto nextV = v == snapshot.size() - 1 ? 0 : v + 1;

		const Vector delta(snapshot[v], snapshot[nextV]);
		Point pt(snapshot[v]);
		pt.move(delta, 1 - rest / llen);
		newPoints.push_back(pt);

		rest -= newEdgeLength;
		while (rest < 0) {
			v = v == snapshot.size() - 1 ? 0 : v + 1;
			llen = edgeLengths[v];
			rest += llen;
		}
	}

	return newPoints;
}

void Knot::normalize(std::size_t newNumberOfPoints) {
	std::lock_guard<std::mutex> writeMethodGuard(this->writeMethodMutex);
	const auto snapshot = this->points();
	auto normalized = this->normalizedPoints(snapshot, newNumberOfPoints);

	counting_lock guard(*this);
	this->_points.swap(normalized);
}

}}
