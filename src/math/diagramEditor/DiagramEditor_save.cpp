#include <sstream>

#include <rapidjson/ostreamwrapper.h>
#include <rapidjson/writer.h>

#include "DiagramEditor.h"

namespace KE { namespace TwoD {

namespace {

std::string docToString(const rapidjson::Document &doc) {
	std::stringstream os;
	rapidjson::OStreamWrapper wrapper(os);
	rapidjson::Writer<rapidjson::OStreamWrapper> writer(wrapper);
	writer.SetMaxDecimalPlaces(5);
	doc.Accept(writer);
	return os.str();
}

}

rapidjson::Document DiagramEditor::serialize() {
	auto doc = this->currentDiagram->serialize();
	this->saveCheckpoint = docToString(doc);
	return doc;
}

bool DiagramEditor::isSaved() const {
	auto doc = this->currentDiagram->serialize();
	return this->saveCheckpoint == docToString(doc);
}

}}
