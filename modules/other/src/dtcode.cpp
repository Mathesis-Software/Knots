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

#include <fstream>
#include <iostream>
#include <list>

#include <rapidjson/document.h>
#include <rapidjson/istreamwrapper.h>

#include <Diagram.h>
#include <DiagramProperty.h>
#include <Util_rapidjson.h>

using namespace KE::TwoD;

int main(int argc, const char **argv) {
	if (argc != 2) {
		std::cerr << "Usage:\n\t" << argv[0] << " <file.dgr>\n";
		return 1;
	}

	rapidjson::Document doc;
	std::ifstream is(argv[1]);
	rapidjson::IStreamWrapper wrapper(is);
	doc.ParseStream(wrapper);
	is.close();
	Diagram diagram(doc);

	for (const auto index : Math::DTCode().value(diagram)) {
		std::cout << index << " ";
	}
	std::cout << "\n";

	return 0;
}
