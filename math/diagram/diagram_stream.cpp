#include <map>
#include <vector>

#include "../../util/rapidjson.h"
#include "diagram.h"

namespace KE { namespace TwoD {

Diagram::Diagram(const rapidjson::Document &doc) : _isClosed(false) {
	if (doc.IsNull()) {
		throw std::runtime_error("The file is not in JSON format");
	}
	if (!doc.IsObject() || Util::rapidjson::get_string(doc, "type") != "diagram") {
		throw std::runtime_error("The file does not represent a diagram");
	}
	this->caption = Util::rapidjson::get_string(doc, "name");
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
		if (!point.IsArray() || point.Size() != 2 || !point[0].IsInt() || !point[1].IsInt()) {
			throw std::runtime_error("Each vertex must be an array of two integers");
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
		throw std::runtime_error("Crossings must be a list of index pairs");
	}
	for (rapidjson::SizeType i = 0; i < crossings.Size(); ++i) {
		const auto &point = crossings[i];
		if (!point.IsArray() || point.Size() != 2 || !point[0].IsInt() || !point[1].IsInt()) {
			throw std::runtime_error("Each crossing an index pair");
		}
		const std::size_t x = point[0].GetInt();
	 	const std::size_t y = point[1].GetInt();
		if (x >= edges.size() || y >= edges.size()) {
			throw std::runtime_error("Vertex index in crossings is out of range");
		}
		this->addCrossing(edges[y], edges[x]);
	}
}

rapidjson::Document Diagram::save() const {
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

	return doc;
}

}}
