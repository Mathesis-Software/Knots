/*
 * Copyright (c) 1995-2021, Nikolay Pultsin <geometer@geometer.name>
 *
 * Licensed under the Apache License, Version 2.0 the "License";
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <numeric>

#include "Knot.h"

namespace KE::ThreeD {

namespace {

std::mutex snapshotMutex;

}

Knot::Snapshot Knot::snapshot() const {
	if (this->lockCount > 0 || !this->latest || this->latest->generation != this->generation) {
		std::lock_guard<std::recursive_mutex> guard(this->dataChangeMutex);
		this->latest = std::shared_ptr<Snapshot>(new Snapshot(*this, this->_points));
	}
	return *this->latest;
}

Knot::Snapshot::Snapshot(const Knot &knot, const std::vector<Point> &points) : knot(knot), points(new std::vector<Point>(points)), _edgeLengths(new std::vector<double>), generation(knot.generation) {
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

double Knot::Snapshot::knotLength() const {
	const auto &edgeLengths = this->edgeLengths();
	return std::accumulate(edgeLengths.begin(), edgeLengths.end(), 0.0);
}

}
