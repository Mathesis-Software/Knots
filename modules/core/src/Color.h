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

#ifndef __KE_COLOR_H__
#define __KE_COLOR_H__

#include <cmath>
#include <memory>
#include <string>

namespace KE {

struct Color {
	float rgb[3];

	Color(int red, int green, int blue) : rgb {red / 255.0f, green / 255.0f, blue / 255.0f} {}

	std::string stringValue() const;

	int red() const { return (int)round(255 * rgb[0]); }
	int green() const { return (int)round(255 * rgb[1]); }
	int blue() const { return (int)round(255 * rgb[2]); }

	static std::shared_ptr<Color> parse(const std::string &stringValue);
	static const Color white;

	bool operator == (const Color &color) const {
		return this->red() == color.red() && this->green() == color.green() && this->blue() == color.blue();
	}
	bool operator != (const Color &color) const { return !(*this == color); }
};

}

#endif /* __KE_COLOR_H__ */
