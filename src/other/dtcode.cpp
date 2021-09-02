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
#include <list>
#include <map>

#include <rapidjson/document.h>
#include <rapidjson/istreamwrapper.h>

#include "../ke/Util_rapidjson.h"
#include "../ke/Diagram.h"

using namespace KE::TwoD;

std::list<int> dtCode(const Diagram &diagram) {
	const auto edges = diagram.edges();
	std::map<std::shared_ptr<Diagram::Vertex>,std::list<Diagram::Crossing>> all_crossings;
	for (const auto &edge : edges) {
		const auto crossings = diagram.crossings(edge);
		auto &list = all_crossings[edge.start];
		list.insert(list.end(), crossings.begin(), crossings.end());
		for (const auto &crs : crossings) {
			all_crossings[crs.up.start].push_back(crs);
		}
	}

	struct Index {
		int odd;
		int even;

		Index() : odd(0), even(0) {}
		void update(int number) {
			if (number % 2) {
				odd = number;
			} else {
				even = number;
			}
		}
	};
	std::map<Diagram::Crossing,Index> indices;
	int count = 1;
	bool start = true;
	int skip = 0;
	for (const auto &edge : edges) {
		auto &crossings = all_crossings[edge.start];
		edge.orderCrossings(crossings);
		for (const auto &cro : crossings) {
			if (start && cro.down == edge) {
				skip += 1;
				continue;
			}
			start = false;
			if (count % 2 == 0 && cro.up == edge) {
				indices[cro].update(-count);
			} else {
				indices[cro].update(count);
			}
			count += 1;
		}
	}
	if (skip > 0) {
		for (const auto &edge : edges) {
			auto &crossings = all_crossings[edge.start];
			for (const auto &cro : crossings) {
				if (count % 2 == 0 && cro.up == edge) {
					indices[cro].update(-count);
				} else {
					indices[cro].update(count);
				}
				count += 1;
				skip -= 1;
				if (skip == 0) {
					break;
				}
			}
			if (skip == 0) {
				break;
			}
		}
	}

	std::list<Index> pairs;
	for (const auto &[cro, index] : indices) {
		pairs.push_back(index);
	}
	pairs.sort([](const Index &i0, const Index &i1) { return i0.odd < i1.odd; });

	std::list<int> code;
	for (const auto &index : pairs) {
		code.push_back(index.even);
	}

	return code;
}

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

	for (const auto index : dtCode(diagram)) {
		std::cout << index << " ";
	}
	std::cout << "\n";

	return 0;
}
