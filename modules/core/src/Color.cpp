/*
 * Copyright (c) 1995-2021, Mathesis Software <mad@mathesis.fun>
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

#include <cmath>
#include <iomanip>
#include <sstream>

#include "Color.h"

namespace KE {

std::string Color::stringValue() const {
	std::stringstream os;
	os
					<< std::hex << std::setfill('0') << std::setw(2) << std::lround(this->rgb[0] * 255)
					<< std::hex << std::setfill('0') << std::setw(2) << std::lround(this->rgb[1] * 255)
					<< std::hex << std::setfill('0') << std::setw(2) << std::lround(this->rgb[2] * 255);
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

}
