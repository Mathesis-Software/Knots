#include <cmath>
#include <map>

#include "knot.h"
#include "../diagram/diagram.h"

namespace KE { namespace ThreeD {

Knot::Knot(const TwoD::Diagram &diagram, int w, int h) : caption(diagram.caption + " (from diagram)") {
	const auto edges = diagram.edges();
	std::map<std::shared_ptr<TwoD::Diagram::Vertex>,std::list<TwoD::Diagram::Crossing>> all_crossings;
	for (const auto &edge : edges) {
		const auto crossings = diagram.crossings(edge);
		auto &list = all_crossings[edge.start];
		list.insert(list.end(), crossings.begin(), crossings.end());
		for (const auto &crs : crossings) {
			all_crossings[crs.up.start].push_back(crs);
		}
	}

	for (const auto &edge : edges) {
		const auto coords = edge.start->coords();
		this->_points.push_back(Point(
			2.4 * coords.x / w - 1.2,
			1.2 - 2.4 * coords.y / h,
			0
		));
		auto &crossings = all_crossings[edge.start];
		edge.orderCrossings(crossings);
		for (const auto &crs : crossings) {
			std::shared_ptr<TwoD::FloatPoint> current = crs.coords();
			if (!current) {
				continue;
			}
			this->_points.push_back(Point(
				2.4 * current->x / w - 1.2,
				1.2 - 2.4 * current->y / h,
				crs.up == edge ? 0.2 : -0.2
			));
		}
	}

  create_depend();

	double min = this->_points.front().distanceTo(this->_points.back());
	double total = min;
	for (std::size_t i = 1; i < this->_points.size(); ++i) {
		const double dist = this->_points[i - 1].distanceTo(this->_points[i]);
		min = std::min(min, dist);
		total += dist;
	}
  normalize(std::max(5 * this->_points.size(), 3 * (std::size_t)std::round(total / min)));
  center();
}

}}
