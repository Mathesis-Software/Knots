#include <cstring>
#include <map>
#include <vector>

#include <rapidjson/document.h>
#include <rapidjson/istreamwrapper.h>
#include <rapidjson/ostreamwrapper.h>
#include <rapidjson/writer.h>

#include "diagram.h"

namespace KE { namespace TwoD {

namespace {
	std::string get_string(const rapidjson::Document &doc, const std::string &key) {
		if (doc.HasMember(key.c_str())) {
			const auto &obj = doc[key.c_str()];
			if (obj.IsString()) {
				return std::string(obj.GetString(), obj.GetStringLength());
			}
		}
		return "";
	}
}

Diagram::Diagram(std::istream &is) : _isClosed(false) {
	rapidjson::Document doc;
	rapidjson::IStreamWrapper wrapper(is);
	doc.ParseStream(wrapper);

	if (!doc.IsObject() || get_string(doc, "type") != "diagram") {
		// throw error
		return;
	}
	this->caption = get_string(doc, "name");
	if (!doc.HasMember("components")) {
		// throw error
		return;
	}
	const auto &components = doc["components"];
	if (!components.IsArray() || components.Size() != 1) {
		// throw error
		return;
	}
	const auto &first = components[0];
	if (!first.IsObject() || !first.HasMember("vertices") || !first.HasMember("crossings")) {
		// throw error
		return;
	}
	const auto &vertices = first["vertices"];
	if (!vertices.IsArray()) {
		// throw error
		return;
	}
	for (rapidjson::SizeType i = 0; i < vertices.Size(); ++i) {
		const auto &point = vertices[i];
		if (!point.IsArray() || point.Size() != 2 || !point[0].IsInt() || !point[1].IsInt()) {
			// throw error
			return;
		}
		this->addVertex(point[0].GetInt(), point[1].GetInt());
	}
	if (doc.HasMember("isClosed")) {
		const auto &closed = doc["isClosed"];
		if (closed.IsBool() && closed.GetBool()) {
			this->close();
		}
	}

	const auto list = this->edges();
	const std::vector<Edge> edges(std::begin(list), std::end(list));
	const auto &crossings = first["crossings"];
	if (!crossings.IsArray()) {
		// throw error
		return;
	}
	for (rapidjson::SizeType i = 0; i < crossings.Size(); ++i) {
		const auto &point = crossings[i];
		if (!point.IsArray() || point.Size() != 2 || !point[0].IsInt() || !point[1].IsInt()) {
			// throw error
			return;
		}
		const int x = point[0].GetInt();
	 	const int y = point[1].GetInt();
		if (x < 0 || x >= edges.size() || y < 0 || y >= edges.size()) {
			// throw error
			return;
		}
		this->addCrossing(edges[y], edges[x]);
	}
}

void Diagram::save(std::ostream &os) {
	rapidjson::Document doc;
	doc.SetObject();
	doc.AddMember("type", "diagram", doc.GetAllocator());
	doc.AddMember("name", rapidjson::StringRef(this->caption.data(), this->caption.size()), doc.GetAllocator());
	doc.AddMember("isClosed", this->isClosed(), doc.GetAllocator());

	rapidjson::Value vertices(rapidjson::kArrayType);
	std::map<std::shared_ptr<Vertex>,std::size_t> nums;
	int index = 0;
	for (const auto &vertex : this->vertices()) {
		nums[vertex] = index;
		index += 1;
		rapidjson::Value point(rapidjson::kArrayType);
   	point.PushBack(vertex->x(), doc.GetAllocator());
   	point.PushBack(vertex->y(), doc.GetAllocator());
		vertices.PushBack(point, doc.GetAllocator());
	}

	rapidjson::Value crossings(rapidjson::kArrayType);
	for (const auto &vertex : this->vertices()) {
    for (const auto &crs : vertex->crossings()) {
			rapidjson::Value c(rapidjson::kArrayType);
			c.PushBack(nums[vertex], doc.GetAllocator());
			c.PushBack(nums[crs.up.start], doc.GetAllocator());
			crossings.PushBack(c, doc.GetAllocator());
		}
  }

	rapidjson::Value components(rapidjson::kArrayType);
	rapidjson::Value first(rapidjson::kObjectType);
	first.AddMember("vertices", vertices, doc.GetAllocator());
	first.AddMember("crossings", crossings, doc.GetAllocator());
	components.PushBack(first, doc.GetAllocator());
	doc.AddMember("components", components, doc.GetAllocator());

	rapidjson::OStreamWrapper wrapper(os);
	rapidjson::Writer<rapidjson::OStreamWrapper> writer(wrapper);
	doc.Accept(writer);
}

}}
