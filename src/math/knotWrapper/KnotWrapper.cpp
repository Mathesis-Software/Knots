#include "KnotWrapper.h"
#include "../../util/rapidjson.h"

namespace KE { namespace ThreeD {

KnotWrapper::KnotWrapper(const TwoD::Diagram &diagram, std::size_t width, std::size_t height) : knot(diagram, width, height) {
	auto cp = this->knot.serialize();
	this->saveCheckpoint = Util::rapidjson::docToString(cp);
}

KnotWrapper::KnotWrapper(const rapidjson::Document &doc) : knot(doc) {
	auto cp = this->knot.serialize();
	this->saveCheckpoint = Util::rapidjson::docToString(cp);
}

rapidjson::Document KnotWrapper::serialize(const double matrix[3][3]) {
	auto doc = this->knot.serialize(matrix);
	this->saveCheckpoint = Util::rapidjson::docToString(doc);
	return doc;
}

bool KnotWrapper::isSaved(const double matrix[3][3]) const {
	auto doc = this->knot.serialize(matrix);
	return this->saveCheckpoint == Util::rapidjson::docToString(doc);
}

}}
