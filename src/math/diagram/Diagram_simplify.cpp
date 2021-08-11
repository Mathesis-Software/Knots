#include <set>
#include <vector>

#include "Diagram.h"

namespace KE { namespace TwoD {

bool Diagram::simplify(std::size_t depth) {
	const auto list = this->edges();
	const std::vector<Edge> edges(std::begin(list), std::end(list));

	if (!this->isClosed() || depth == 0 || edges.size() <= 2 * depth) {
		return false;
	}

	std::set<std::shared_ptr<Vertex>> crossings;
	for (const auto &edge : edges) {
		for (const auto &crs : this->crossings(edge)) {
			crossings.insert(crs.up.start);
			crossings.insert(crs.down.start);
		}
	}

	std::vector<bool> single_flags;
	for (const auto edge : edges) {
		single_flags.push_back(crossings.find(edge.start) == crossings.end());
	}
	for (std::size_t i = 0; i < 2 * depth; ++i) {
		single_flags.push_back(single_flags[i]);
	}

	std::vector<bool> depth_flags;
	std::size_t count = 0;
	for (std::size_t index = 0; index < single_flags.size(); ++index) {
		if (single_flags[index]) {
			count += 1;
		} else {
			count = 0;
		}
		depth_flags.push_back(count >= 2 * depth);
	}
	for (std::size_t i = 0; i < 2 * depth; ++i) {
		depth_flags[i] = depth_flags[i + edges.size()];
	}

	std::size_t size = edges.size();
	for (std::size_t index = 0; index < edges.size(); ++index) {
		if (size <= 3) {
			break;
		}
		if (depth_flags[index + depth - 1]) {
			this->removeVertex(edges[index].start);
			--size;
		}
	}

	return size != edges.size();
}

}}
