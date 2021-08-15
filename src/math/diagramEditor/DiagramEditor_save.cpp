#include "DiagramEditor.h"
#include "../../util/rapidjson.h"

namespace KE { namespace TwoD {

rapidjson::Document DiagramEditor::serialize() {
	auto doc = this->currentDiagram->serialize();
	this->saveCheckpoint = Util::rapidjson::docToString(doc);
	return doc;
}

bool DiagramEditor::isSaved() const {
	auto doc = this->currentDiagram->serialize();
	return this->saveCheckpoint == Util::rapidjson::docToString(doc);
}

}}
