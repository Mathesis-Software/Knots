#include <rapidjson/document.h>
#include <rapidjson/istreamwrapper.h>
#include <rapidjson/ostreamwrapper.h>
#include <rapidjson/writer.h>

#include "knot.h"

namespace KE { namespace ThreeD {

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

Knot::Knot(std::istream &is) {
	rapidjson::Document doc;
	rapidjson::IStreamWrapper wrapper(is);
	doc.ParseStream(wrapper);

	if (!doc.IsObject() || get_string(doc, "type") != "link") {
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
	if (!first.IsArray() || first.Size() < 3) {
		// throw error
		return;
	}
	for (rapidjson::SizeType i = 0; i < first.Size(); ++i) {
		const auto &point = first[i];
		if (!point.IsArray() || point.Size() != 3 || !point[0].IsNumber() || !point[1].IsNumber() || !point[2].IsNumber()) {
			// throw error
			return;
		}
		this->points.push_back(Point(point[0].GetDouble(), point[1].GetDouble(), point[2].GetDouble()));
	}
}

void Knot::save(std::ostream &os) const {
	double matrix[3][3] = {
		{1.0, 0.0, 0.0},
		{0.0, 1.0, 0.0},
		{0.0, 0.0, 1.0}
	};
	this->save(os, matrix);
}

void Knot::save(std::ostream &os, const double matrix[3][3]) const {
	rapidjson::Document doc;
	doc.SetObject();
	doc.AddMember("type", "link", doc.GetAllocator());
	doc.AddMember("name", rapidjson::StringRef(this->caption.data(), this->caption.size()), doc.GetAllocator());
	rapidjson::Value first(rapidjson::kArrayType);
	for (const auto &pt : this->points) {
		rapidjson::Value point(rapidjson::kArrayType);
		for (std::size_t i = 0; i < 3; ++i) {
    	point.PushBack(
				matrix[0][i] * pt.x +
				matrix[1][i] * pt.y +
				matrix[2][i] * pt.z,
				doc.GetAllocator()
			);
		}
		first.PushBack(point, doc.GetAllocator());
	}
	rapidjson::Value components(rapidjson::kArrayType);
	components.PushBack(first, doc.GetAllocator());
	doc.AddMember("components", components, doc.GetAllocator());

	rapidjson::OStreamWrapper wrapper(os);
	rapidjson::Writer<rapidjson::OStreamWrapper> writer(wrapper);
	writer.SetMaxDecimalPlaces(5);
	doc.Accept(writer);
}

}}
