#include <math.h>

#include "knot.h"

namespace KE { namespace ThreeD {

std::vector<Point> Knot::normalizedPoints(std::size_t newNumberOfPoints) const {
	const auto points = this->points();
	std::vector<Vector> edges;
	edges.reserve(points.size());
	for (std::size_t i = 0; i < points.size() - 1; ++i) {
		edges.push_back(Vector(points[i], points[i + 1]));
	}
	edges.push_back(Vector(points[points.size() - 1], points[0]));

	double totalLength = 0.0;
	for (const auto &edge : edges) {
		totalLength += edge.length();
	}
	const double newEdgeLength = totalLength / newNumberOfPoints;

	std::vector<Point> newPoints;
	newPoints.reserve(newNumberOfPoints);

	{
		std::size_t v = 0;
		double llen = edges[0].length();
		double rest = llen;

		for (std::size_t i = 0; i < newNumberOfPoints; i++) {
			const auto nextV = v == points.size() - 1 ? 0 : v + 1;

			const Vector delta(points[v], points[nextV]);
			Point pt(points[v]);
			pt.move(delta, 1 - rest / llen);
			newPoints.push_back(pt);

			rest -= newEdgeLength;
			while (rest < 0) {
				v = v == points.size() - 1 ? 0 : v + 1;
				llen = edges[v].length();
				rest += llen;
			}
		}
	}

	return newPoints;
}

void Knot::normalize(std::size_t newNumberOfPoints) {
	std::lock_guard<std::mutex> writeMethodGuard(this->writeMethodMutex);
	auto normalized = this->normalizedPoints(newNumberOfPoints);

	counting_lock guard(*this);
	this->_points.swap(normalized);
}

}}
