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

#include <iostream>
#include <fstream>

#include <rapidjson/document.h>
#include <rapidjson/istreamwrapper.h>
#include <rapidjson/ostreamwrapper.h>
#include <rapidjson/writer.h>

#include <Util_rapidjson.h>
#include <DiagramEditor.h>
#include <KnotWrapper.h>

void convert(const std::string &filename) {
	rapidjson::Document doc;
	{
		std::ifstream is(filename);
		rapidjson::IStreamWrapper wrapper(is);
		doc.ParseStream(wrapper);
		is.close();
	}

	const std::string type = KE::Util::rapidjson::getString(doc, "type");
	if (type == "link") {
		KE::ThreeD::KnotWrapper knot(doc);
		doc = knot.serialize();
	} else if (type == "diagram") {
		KE::TwoD::DiagramEditor diagram(doc);
		doc = diagram.serialize();
	} else {
		throw std::runtime_error("Unknown file type");
	}

	{
		std::ofstream os(filename);
		rapidjson::OStreamWrapper wrapper(os);
		rapidjson::Writer<rapidjson::OStreamWrapper> writer(wrapper);
		writer.SetMaxDecimalPlaces(5);
		doc.Accept(writer);
		os.close();
	}
}

int main(int argc, const char **argv) {
	for (int i = 1; i < argc; ++i) {
		convert(argv[i]);
	}
	return 0;
}
