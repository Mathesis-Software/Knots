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

#include <iostream>
#include <fstream>

#include <rapidjson/document.h>
#include <rapidjson/istreamwrapper.h>

#include "../ke/Util_rapidjson.h"
#include "../ke/KnotWrapper.h"
#include "../math/computables.h"

int main(int argc, const char **argv) {
	if (argc != 2) {
		std::cerr << "Usage:\n\t" << argv[0] << " <file.knt>\n";
		return 1;
	}

	rapidjson::Document doc;
	std::ifstream is(argv[1]);
	rapidjson::IStreamWrapper wrapper(is);
	doc.ParseStream(wrapper);
	is.close();
	KE::ThreeD::KnotWrapper knot(doc);
	for (std::size_t order = 1; order < 10; ++order) {
		KE::ThreeD::Math::VassilievInvariant invariant(knot, order);
		std::cout << invariant.name << ": " << invariant.value() << "\n";
	}

	return 0;
}
