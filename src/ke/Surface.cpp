/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at

 *   http://www.apache.org/licenses/LICENSE-2.0

 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 *
 * Author: Nikolay Pultsin <geometer@geometer.name>
 */

#include "Surface.h"

namespace KE::GL {

Surface::Surface(bool stripped, bool showBackSide) : stripped(stripped), showBackSide(showBackSide) {}

Surface::~Surface() {
}

bool Surface::isObsolete() const {
	return this->_points.empty();
}

void Surface::destroy() {
	this->_points.clear();
}

void Surface::addpoint(const ThreeD::Point &vertex, const ThreeD::Vector &normal) const {
	this->_points.push_back(SurfacePoint(vertex.x, vertex.y, vertex.z, normal.x, normal.y, normal.z));
}

void Surface::prepare() const {
	if (this->isObsolete()) {
		this->_points.clear();
	}
	if (this->_points.empty()) {
		this->calculate();
	}
}

}
