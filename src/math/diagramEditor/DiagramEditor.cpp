#include "DiagramEditor.h"

namespace KE { namespace TwoD {

std::shared_ptr<Diagram::Vertex> DiagramEditor::addVertex(int x, int y) {
	return this->_diagram.addVertex(x, y);
}

std::shared_ptr<Diagram::Vertex> DiagramEditor::addVertex(const Diagram::Edge &edge, int x, int y) {
	return this->_diagram.addVertex(edge, x, y);
}

void DiagramEditor::removeVertex(const std::shared_ptr<Diagram::Vertex> &vertex) {
	this->_diagram.removeVertex(vertex);
}

void DiagramEditor::moveVertex(const std::shared_ptr<Diagram::Vertex> &vertex, int x, int y) {
	this->_diagram.moveVertex(vertex, x, y);
}

std::shared_ptr<Diagram::Crossing> DiagramEditor::flipCrossing(Diagram::Crossing &crossing) {
	return this->_diagram.flipCrossing(crossing);
}

void DiagramEditor::shift(int dx, int dy) {
	this->_diagram.shift(dx, dy);
}

void DiagramEditor::close() {
	return this->_diagram.close();
}

void DiagramEditor::clear() {
	return this->_diagram.clear();
}

bool DiagramEditor::simplify() {
	return this->_diagram.simplify(2);
}

}}
