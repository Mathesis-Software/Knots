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

#include <iomanip>
#include <sstream>

#include "Surface.h"

namespace KE { namespace GL {

Surface::Surface(bool stripped, bool showBackSide) : stripped(stripped), showBackSide(showBackSide) {}

Surface::~Surface() {
}

bool Surface::destroy(bool force) {
	if (force) {
		this->points.clear();
	}
	return force;
}

void Surface::addpoint(const ThreeD::Point &vertex, const ThreeD::Vector &normal) {
	this->points.push_back(SurfacePoint(vertex.x, vertex.y, vertex.z, normal.x, normal.y, normal.z));
}

std::string Color::stringValue() const {
	std::stringstream os;
	os
		<< std::hex << std::setfill('0') << std::setw(2) << (int)round(this->rgb[0] * 255)
		<< std::hex << std::setfill('0') << std::setw(2) << (int)round(this->rgb[1] * 255)
		<< std::hex << std::setfill('0') << std::setw(2) << (int)round(this->rgb[2] * 255);
	return os.str();
}

const Color Color::white(255, 255, 255);

std::shared_ptr<Color> Color::parse(const std::string &stringValue) {
	if (stringValue.size() != 6) {
		return nullptr;
	}
	unsigned int value;
	std::stringstream stream;
	stream << std::hex << stringValue;
	stream >> value;
	return std::make_shared<Color>((value >> 16) & 0xff, (value >> 8) & 0xff, value & 0xff);
}

}}
