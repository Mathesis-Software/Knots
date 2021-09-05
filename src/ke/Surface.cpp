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

#include "Surface.h"

namespace KE::GL {

Surface::Surface(bool stripped, bool showBackSide) : stripped(stripped), showBackSide(showBackSide) {}

Surface::~Surface() {
}

bool Surface::isObsolete() const {
	return this->points.empty();
}

void Surface::destroy() {
	this->points.clear();
}

void Surface::addpoint(const ThreeD::Point &vertex, const ThreeD::Vector &normal) const {
	this->points.push_back(SurfacePoint(vertex.x, vertex.y, vertex.z, normal.x, normal.y, normal.z));
}

}
