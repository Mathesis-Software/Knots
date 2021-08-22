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

#include "Util_rapidjson.h"
#include "Knot.h"

namespace KE::ThreeD {

Knot::Knot(const rapidjson::Document &doc) : generation(1), lockCount(0) {
	if (doc.IsNull()) {
		throw std::runtime_error("The file is not in JSON format");
	}
	if (!doc.IsObject() || Util::rapidjson::getString(doc, "type") != "link") {
		throw std::runtime_error("The file does not represent a knot");
	}
	this->caption = Util::rapidjson::getString(doc, "name");
	if (!doc.HasMember("components")) {
		throw std::runtime_error("The file contains no diagram components");
	}
	const auto &components = doc["components"];
	if (!components.IsArray()) {
		throw std::runtime_error("Components format is incorrect: a list expected");
	}
	if (components.Size() != 1) {
		throw std::runtime_error("The app does not support multi-component diagrams");
	}
	const auto &first = components[0];
	if (!first.IsObject() || !first.HasMember("points")) {
		throw std::runtime_error("Component format incorrect: expected an object containing points member");
	}
	const auto &points = first["points"];
	if (!points.IsArray() || points.Size() < 3) {
		throw std::runtime_error("Points format incorrect: expected a list of at least three elements");
	}
	for (rapidjson::SizeType i = 0; i < points.Size(); ++i) {
		const auto &point = points[i];
		if (!point.IsArray() || point.Size() != 3 || !point[0].IsNumber() || !point[1].IsNumber() || !point[2].IsNumber()) {
			throw std::runtime_error("Each point must be an array of three integers");
		}
		this->_points.push_back(Point(point[0].GetDouble(), point[1].GetDouble(), point[2].GetDouble()));
	}
}

rapidjson::Document Knot::serialize() const {
	rapidjson::Document doc;
	doc.SetObject();
	doc.AddMember("type", "link", doc.GetAllocator());
	rapidjson::Value caption;
	caption.SetString(this->caption.data(), this->caption.size(), doc.GetAllocator());
	doc.AddMember("name", caption, doc.GetAllocator());
	rapidjson::Value components(rapidjson::kArrayType);

	rapidjson::Value first(rapidjson::kObjectType);
	rapidjson::Value points(rapidjson::kArrayType);
	const auto pts = this->snapshot();
	for (std::size_t i = 0; i < pts.size(); ++i) {
		rapidjson::Value point(rapidjson::kArrayType);
		point.PushBack(pts[i].x, doc.GetAllocator());
		point.PushBack(pts[i].y, doc.GetAllocator());
		point.PushBack(pts[i].z, doc.GetAllocator());
		points.PushBack(point, doc.GetAllocator());
	}
	first.AddMember("points", points, doc.GetAllocator());

	components.PushBack(first, doc.GetAllocator());

	doc.AddMember("components", components, doc.GetAllocator());

	return doc;
}

}
