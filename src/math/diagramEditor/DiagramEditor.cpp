#include "DiagramEditor.h"

namespace KE { namespace TwoD {

bool DiagramEditor::canUndo() const {
	return true;
}

bool DiagramEditor::canRedo() const {
	return false;
}

void DiagramEditor::undo() {
	auto replacement = std::make_shared<Diagram>(this->initialDiagram.save());
	this->currentDiagram = replacement;
}

void DiagramEditor::redo() {
}

std::shared_ptr<Diagram::Vertex> DiagramEditor::addVertex(int x, int y) {
	return this->currentDiagram->addVertex(x, y);
}

std::shared_ptr<Diagram::Vertex> DiagramEditor::addVertex(const Diagram::Edge &edge, int x, int y) {
	return this->currentDiagram->addVertex(edge, x, y);
}

void DiagramEditor::removeVertex(const std::shared_ptr<Diagram::Vertex> &vertex) {
	this->currentDiagram->removeVertex(vertex);
}

void DiagramEditor::moveVertex(const std::shared_ptr<Diagram::Vertex> &vertex, int x, int y) {
	this->currentDiagram->moveVertex(vertex, x, y);
}

std::shared_ptr<Diagram::Crossing> DiagramEditor::flipCrossing(Diagram::Crossing &crossing) {
	return this->currentDiagram->flipCrossing(crossing);
}

void DiagramEditor::shift(int dx, int dy) {
	this->currentDiagram->shift(dx, dy);
}

void DiagramEditor::close() {
	return this->currentDiagram->close();
}

void DiagramEditor::clear() {
	return this->currentDiagram->clear();
}

bool DiagramEditor::simplify() {
	return this->currentDiagram->simplify(2);
}

}}
