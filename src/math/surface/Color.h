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

#ifndef __KE_COLOR_H__
#define __KE_COLOR_H__

#include <memory>
#include <string>

namespace KE {

struct Color {
	float rgb[3];

	Color(int red, int green, int blue) : rgb {red / 255.0f, green / 255.0f, blue / 255.0f} {}

	std::string stringValue() const;

	static std::shared_ptr<Color> parse(const std::string &stringValue);
	static const Color white;
};

}

#endif /* __KE_COLOR_H__ */
