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

#include <map>
#include <vector>

#include "Util_rapidjson.h"
#include "Diagram.h"

namespace KE::TwoD {

Diagram::Diagram(const rapidjson::Document &doc) : _isClosed(false) {
	if (doc.IsNull()) {
		throw std::runtime_error("The file is not in JSON format");
	}
	if (!doc.IsObject() || Util::rapidjson::getString(doc, "type") != "diagram") {
		throw std::runtime_error("The file does not represent a diagram");
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
	if (!first.IsObject() || !first.HasMember("vertices") || !first.HasMember("crossings")) {
		throw std::runtime_error("Component format is incorrect: `vertices` and `crossings` fields expected");
	}
	const auto &vertices = first["vertices"];
	if (!vertices.IsArray()) {
		throw std::runtime_error("Vertices must be a list of points");
	}
	for (rapidjson::SizeType i = 0; i < vertices.Size(); ++i) {
		const auto &point = vertices[i];
		if (!point.IsArray() || point.Size() != 3 || !point[0].IsInt() || !point[1].IsInt()) {
			throw std::runtime_error("Each vertex must be an array of three integers");
		}
		this->addVertex(point[1].GetInt(), point[2].GetInt(), point[0].GetInt());
	}
	if (first.HasMember("isClosed")) {
		const auto &closed = first["isClosed"];
		if (closed.IsBool() && closed.GetBool()) {
			this->close();
		}
	}

	const auto list = this->edges();
	const std::vector<Edge> edges(std::begin(list), std::end(list));
	const auto &crossings = first["crossings"];
	if (!crossings.IsArray()) {
		throw std::runtime_error("Crossings must be a list of objects");
	}
	for (rapidjson::SizeType i = 0; i < crossings.Size(); ++i) {
		const auto &point = crossings[i];
		if (!point.IsObject() || !point.HasMember("up") || !point.HasMember("down")) {
			throw std::runtime_error("Each crossing must have up and down fields");
		}
		const auto &up = point["up"];
		if (!up.IsInt()) {
			throw std::runtime_error("Crossing up must be an integer");
		}
		const auto &down = point["down"];
		if (!down.IsInt()) {
			throw std::runtime_error("Crossing down must be an integer");
		}
		const std::size_t up_int = up.GetInt();
		const std::size_t down_int = down.GetInt();
		if (up_int >= edges.size() || down_int >= edges.size()) {
			throw std::runtime_error("Vertex index in crossings is out of range");
		}
		this->addCrossing(edges[up_int], edges[down_int]);
	}
}

rapidjson::Document Diagram::serialize() const {
	rapidjson::Document doc;
	doc.SetObject();
	doc.AddMember("type", "diagram", doc.GetAllocator());
	rapidjson::Value caption;
	caption.SetString(this->caption.data(), this->caption.size(), doc.GetAllocator());
	doc.AddMember("name", caption, doc.GetAllocator());

	rapidjson::Value vertices(rapidjson::kArrayType);
	std::map<std::shared_ptr<Vertex>,std::size_t> nums;
	int index = 0;
	for (const auto &vertex : this->vertices()) {
		nums[vertex] = index;
		index += 1;
		rapidjson::Value point(rapidjson::kArrayType);
		point.PushBack((int)vertex->index, doc.GetAllocator());
		point.PushBack(vertex->_x, doc.GetAllocator());
		point.PushBack(vertex->_y, doc.GetAllocator());
		vertices.PushBack(point, doc.GetAllocator());
	}

	rapidjson::Value crossings(rapidjson::kArrayType);
	for (const auto &edge : this->edges()) {
		for (const auto &crs : this->underCrossings(edge)) {
			rapidjson::Value c(rapidjson::kObjectType);
			c.AddMember("down", (int)nums[edge.start], doc.GetAllocator());
			c.AddMember("up", (int)nums[crs.up.start], doc.GetAllocator());
			crossings.PushBack(c, doc.GetAllocator());
		}
	}

	rapidjson::Value components(rapidjson::kArrayType);
	rapidjson::Value first(rapidjson::kObjectType);
	first.AddMember("vertices", vertices, doc.GetAllocator());
	first.AddMember("crossings", crossings, doc.GetAllocator());
	first.AddMember("isClosed", this->isClosed(), doc.GetAllocator());
	components.PushBack(first, doc.GetAllocator());
	doc.AddMember("components", components, doc.GetAllocator());

	return doc;
}

}
