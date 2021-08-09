#include "../../util/rapidjson.h"
#include "knot.h"

namespace KE { namespace ThreeD {

Knot::Knot(const rapidjson::Document &doc) : generation(0), lockCount(0) {
	if (doc.IsNull()) {
		throw std::runtime_error("The file is not in JSON format");
	}
	if (!doc.IsObject() || Util::rapidjson::get_string(doc, "type") != "link") {
		throw std::runtime_error("The file does not represent a knot");
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
	if (!first.IsArray() || first.Size() < 3) {
		throw std::runtime_error("Component format is incorrect: expected a list of at least three points");
	}
	for (rapidjson::SizeType i = 0; i < first.Size(); ++i) {
		const auto &point = first[i];
		if (!point.IsArray() || point.Size() != 3 || !point[0].IsNumber() || !point[1].IsNumber() || !point[2].IsNumber()) {
			throw std::runtime_error("Each point must be an array of three integers");
		}
		this->_points.push_back(Point(point[0].GetDouble(), point[1].GetDouble(), point[2].GetDouble()));
	}

	this->create_depend();
}

rapidjson::Document Knot::save() const {
	double matrix[3][3] = {
		{1.0, 0.0, 0.0},
		{0.0, 1.0, 0.0},
		{0.0, 0.0, 1.0}
	};
	return this->save(matrix);
}

rapidjson::Document Knot::save(const double matrix[3][3]) const {
	rapidjson::Document doc;
	doc.SetObject();
	doc.AddMember("type", "link", doc.GetAllocator());
	doc.AddMember("name", rapidjson::StringRef(this->caption.data(), this->caption.size()), doc.GetAllocator());
	rapidjson::Value first(rapidjson::kArrayType);
	const auto points = this->points();
	for (std::size_t i = 0; i < points.size(); ++i) {
		rapidjson::Value point(rapidjson::kArrayType);
		for (std::size_t j = 0; j < 3; ++j) {
    	point.PushBack(
				matrix[0][j] * points[i].x +
				matrix[1][j] * points[i].y +
				matrix[2][j] * points[i].z,
				doc.GetAllocator()
			);
		}
		first.PushBack(point, doc.GetAllocator());
	}
	rapidjson::Value components(rapidjson::kArrayType);
	components.PushBack(first, doc.GetAllocator());
	doc.AddMember("components", components, doc.GetAllocator());

	return doc;
}

}}
