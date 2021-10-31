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
#include <iostream>
#include <numeric>
#include <sstream>

#include <rapidjson/document.h>
#include <rapidjson/ostreamwrapper.h>
#include <rapidjson/writer.h>

#include <Knot.h>

void print_usage(const std::string argv0) {
	std::cerr << "usage:\n\t" << argv0 << " <number of points> <meredian number> <longitude number>\n";
}

int main(int argc, char **argv) {
	if (argc != 4) {
		print_usage(argv[0]);
		return 1;
	}

	const int numberOfPoints = std::stoi(argv[1]);
	const int meridian = std::stoi(argv[2]);
	const int longitude = std::stoi(argv[3]);

	if (numberOfPoints < 1) {
		std::cerr << "Error: number of points (" << numberOfPoints << ") must be a positive integer.\n";
		return 1;
	}

	const int gcd = std::gcd(meridian, longitude);
	if (gcd != 1) {
		std::cerr << "Error: meridian (" << meridian << ") and longitude (" << longitude << ") numbers must be coprime.\n";
		return 1;
	}

	std::vector<KE::ThreeD::Point> points;
	for (int i = 0; i < numberOfPoints; ++i) {
		const double lonAngle = 2 * M_PI * longitude * i / numberOfPoints;
		const double merAngle = 2 * M_PI * meridian * i / numberOfPoints;
		points.push_back(KE::ThreeD::Point(
			sin(lonAngle) * (0.7 + 0.3 * cos(merAngle)),
			cos(lonAngle) * (0.7 + 0.3 * cos(merAngle)),
			0.3 * sin(merAngle)
		));
	}

	std::stringstream name;
	name << "Torus " << meridian << "x" << longitude;
	KE::ThreeD::Knot knot(points, name.str());
	knot.normalize(numberOfPoints);
	knot.center();
	const rapidjson::Document doc = knot.serialize();

	rapidjson::OStreamWrapper wrapper(std::cout);
	rapidjson::Writer<rapidjson::OStreamWrapper> writer(wrapper);
	writer.SetMaxDecimalPlaces(5);
	doc.Accept(writer);

	return 0;
}
