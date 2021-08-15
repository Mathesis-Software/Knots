#include "KnotWrapper.h"
#include "../../util/rapidjson.h"

namespace KE { namespace ThreeD {

KnotWrapper::KnotWrapper(const TwoD::Diagram &diagram, std::size_t width, std::size_t height) : knot(diagram, width, height) {
	auto cp = this->knot.serialize();
	this->saveUiOptions(cp);
	this->saveCheckpoint = Util::rapidjson::docToString(cp);
}

KnotWrapper::KnotWrapper(const rapidjson::Document &doc) : knot(doc) {
	this->readUiOptions(doc);
	auto cp = this->knot.serialize();
	this->saveUiOptions(cp);
	this->saveCheckpoint = Util::rapidjson::docToString(cp);
}

rapidjson::Document KnotWrapper::serialize(const double matrix[3][3]) {
	auto doc = this->knot.serialize(matrix);
	this->saveUiOptions(doc);
	this->saveCheckpoint = Util::rapidjson::docToString(doc);
	return doc;
}

bool KnotWrapper::isSaved(const double matrix[3][3]) const {
	auto doc = this->knot.serialize(matrix);
	this->saveUiOptions(doc);
	return this->saveCheckpoint == Util::rapidjson::docToString(doc);
}

void KnotWrapper::saveUiOptions(rapidjson::Document &doc) const {
	rapidjson::Value componentUi(rapidjson::kObjectType);
	if (this->knotColor) {
		const std::string sv = this->knotColor->stringValue();
		rapidjson::Value color;
		color.SetString(sv.data(), sv.size(), doc.GetAllocator());
		componentUi.AddMember("color", color, doc.GetAllocator());
	}
	doc["components"][0].AddMember("ui", componentUi, doc.GetAllocator());
}

void KnotWrapper::readUiOptions(const rapidjson::Document &doc) {
	const auto &componentRoot = doc["components"][0];
	if (componentRoot.HasMember("ui") && componentRoot["ui"].IsObject()) {
		const auto &componentUi = componentRoot["ui"];
		this->knotColor = GL::Color::parse(Util::rapidjson::getString(componentUi, "color"));
	}
}

}}
