#include "DiagramEditor.h"

namespace KE { namespace TwoD {

namespace {

struct SavePointCommand : public DiagramEditor::Command {
	void play(Diagram&) override {
	}
};

std::shared_ptr<DiagramEditor::Command> savePointCommand(new SavePointCommand());

}

void DiagramEditor::savePoint() {
	if (this->indexInLog > 0 && this->log[this->indexInLog - 1] != savePointCommand) {
		this->trimLog();
		this->log.push_back(savePointCommand);
		this->indexInLog += 1;
	}
}

void DiagramEditor::trimLog() {
	if (this->indexInLog < this->log.size()) {
		this->log.erase(this->log.begin() + this->indexInLog, this->log.end());
	}
}

void DiagramEditor::addCommand(const std::shared_ptr<Command> &command, bool savePoint) {
	this->trimLog();
	this->log.push_back(command);
	this->indexInLog += 1;
	if (savePoint) {
		this->savePoint();
	}
}

bool DiagramEditor::canUndo() const {
	return this->indexInLog > 0;
}

bool DiagramEditor::canRedo() const {
	return this->indexInLog < this->log.size();
}

void DiagramEditor::undo() {
	if (this->indexInLog == 0) {
		return;
	}
	for (this->indexInLog -= 1; this->indexInLog > 0; this->indexInLog -= 1) {
		if (this->log[this->indexInLog - 1] == savePointCommand) {
			break;
		}
	}
	auto replacement = std::make_shared<Diagram>(this->initialDiagram.save());
	for (std::size_t index = 0; index < this->indexInLog; index += 1) {
		this->log[index]->play(*replacement);
	}
	this->currentDiagram = replacement;
}

void DiagramEditor::redo() {
	for (; this->indexInLog < this->log.size(); this->indexInLog += 1) {
		const auto &command = this->log[this->indexInLog];
		if (command == savePointCommand) {
			this->indexInLog += 1;
			break;
		}
		command->play(*this->currentDiagram);
	}
}

namespace {

struct AddVertexCommand : public DiagramEditor::Command {
	const int x, y;

	AddVertexCommand(int x, int y) : x(x), y(y) {}

	void play(Diagram &diagram) override {
		diagram.addVertex(this->x, this->y);
	}
};

}

std::shared_ptr<Diagram::Vertex> DiagramEditor::addVertex(int x, int y) {
	auto command = std::make_shared<AddVertexCommand>(x, y);
	this->addCommand(command, false);
	return this->currentDiagram->addVertex(x, y);
}

namespace {

template<typename T>
std::size_t indexOf(const T &element, const std::list<T> &collection) {
	std::size_t index = 0;
	for (const auto &elt : collection) {
		if (elt == element) {
			break;
		}
		index += 1;
	}
	return index;
}

template<typename T>
const T& elementAt(std::size_t index, const std::list<T> &collection) {
	for (const auto &elt : collection) {
		if (index == 0) {
			return elt;
		}
		index -= 1;
	}
	throw std::runtime_error("Index out of range");
}

struct AddVertexOnEdgeCommand : public DiagramEditor::Command {
	const std::size_t indexOfEdge;
	const int x, y;

	AddVertexOnEdgeCommand(std::size_t indexOfEdge, int x, int y) : indexOfEdge(indexOfEdge), x(x), y(y) {}

	void play(Diagram &diagram) override {
		diagram.addVertex(elementAt(this->indexOfEdge, diagram.edges()), this->x, this->y);
	}
};

}

std::shared_ptr<Diagram::Vertex> DiagramEditor::addVertex(const Diagram::Edge &edge, int x, int y) {
	this->addCommand(
		std::make_shared<AddVertexOnEdgeCommand>(indexOf(edge, this->currentDiagram->edges()), x, y),
		false
	);
	return this->currentDiagram->addVertex(edge, x, y);
}

namespace {

struct RemoveVertexCommand : public DiagramEditor::Command {
	const std::size_t indexOfVertex;

	RemoveVertexCommand(std::size_t indexOfVertex) : indexOfVertex(indexOfVertex) {}

	void play(Diagram &diagram) override {
		diagram.removeVertex(elementAt(this->indexOfVertex, diagram.vertices()));
	}
};

}

bool DiagramEditor::canRemoveVertex(const std::shared_ptr<Diagram::Vertex>&) const {
	return !this->currentDiagram->isClosed() || this->currentDiagram->vertices().size() > 3;
}

void DiagramEditor::removeVertex(const std::shared_ptr<Diagram::Vertex> &vertex) {
	this->addCommand(
		std::make_shared<RemoveVertexCommand>(indexOf(vertex, this->currentDiagram->vertices())),
		true
	);
	this->currentDiagram->removeVertex(vertex);
}

namespace {

struct RemoveEdgeCommand : public DiagramEditor::Command {
	const std::size_t indexOfEdge;

	RemoveEdgeCommand(std::size_t indexOfEdge) : indexOfEdge(indexOfEdge) {}

	void play(Diagram &diagram) override {
		diagram.removeEdge(elementAt(this->indexOfEdge, diagram.edges()));
	}
};

}

bool DiagramEditor::canRemoveEdge(const std::shared_ptr<Diagram::Edge> &edge) const {
	if (this->currentDiagram->isClosed()) {
	 	return true;
	}
	const auto edges = this->currentDiagram->edges();
	return *edge == edges.front() || *edge == edges.back();
}

void DiagramEditor::removeEdge(const std::shared_ptr<Diagram::Edge> &edge) {
	this->addCommand(
		std::make_shared<RemoveEdgeCommand>(indexOf(*edge, this->currentDiagram->edges())),
		true
	);
	this->currentDiagram->removeEdge(*edge);
}

namespace {

struct MoveVertexCommand : public DiagramEditor::Command {
	const std::size_t indexOfVertex;
	const int x, y;

	MoveVertexCommand(std::size_t indexOfVertex, int x, int y) : indexOfVertex(indexOfVertex), x(x), y(y) {}

	void play(Diagram &diagram) override {
		diagram.moveVertex(elementAt(this->indexOfVertex, diagram.vertices()), this->x, this->y);
	}
};

}

void DiagramEditor::moveVertex(const std::shared_ptr<Diagram::Vertex> &vertex, int x, int y, bool storeCommand) {
	if (storeCommand) {
		this->addCommand(
			std::make_shared<MoveVertexCommand>(indexOf(vertex, this->currentDiagram->vertices()), x, y),
			false
		);
	}
	this->currentDiagram->moveVertex(vertex, x, y);
}

namespace {

struct FlipCrossingCommand : public DiagramEditor::Command {
	const std::size_t indexUp, indexDown;

	FlipCrossingCommand(std::size_t indexUp, std::size_t indexDown) : indexUp(indexUp), indexDown(indexDown) {}

	void play(Diagram &diagram) override {
		const auto edges = diagram.edges();
		auto crossing = diagram.getCrossing(
			elementAt(this->indexUp, edges), elementAt(this->indexDown, edges)
		);
		diagram.flipCrossing(*crossing);
	}
};

}

std::shared_ptr<Diagram::Crossing> DiagramEditor::flipCrossing(Diagram::Crossing &crossing) {
	const auto edges = this->currentDiagram->edges();
	this->addCommand(
		std::make_shared<FlipCrossingCommand>(
			indexOf(crossing.up, edges), indexOf(crossing.down, edges)
		),
		true
	);
	return this->currentDiagram->flipCrossing(crossing);
}

namespace {

struct MoveDiagramCommand : public DiagramEditor::Command {
	const int dx, dy;

	MoveDiagramCommand(int dx, int dy) : dx(dx), dy(dy) {}

	void play(Diagram &diagram) override {
		diagram.shift(dx, dy);
	}
};

}

void DiagramEditor::shift(int dx, int dy, bool storeCommand) {
	if (storeCommand) {
		this->addCommand(std::make_shared<MoveDiagramCommand>(dx, dy), false);
	}
	this->currentDiagram->shift(dx, dy);
}

namespace {

struct CloseCommand : public DiagramEditor::Command {
	void play(Diagram &diagram) override {
		diagram.close();
	}
};
std::shared_ptr<DiagramEditor::Command> closeCommand(new CloseCommand());

}

void DiagramEditor::close() {
	this->addCommand(closeCommand, false);
	closeCommand->play(*this->currentDiagram);
}

namespace {

struct ClearCommand : public DiagramEditor::Command {
	void play(Diagram &diagram) override {
		diagram.clear();
	}
};
std::shared_ptr<DiagramEditor::Command> clearCommand(new ClearCommand());

}

void DiagramEditor::clear() {
	this->addCommand(clearCommand, false);
	clearCommand->play(*this->currentDiagram);
}

namespace {

struct SimplifyCommand : public DiagramEditor::Command {
	void play(Diagram &diagram) override {
		diagram.simplify(2);
	}
};
std::shared_ptr<DiagramEditor::Command> simplifyCommand(new SimplifyCommand());

}

bool DiagramEditor::simplify() {
	if (this->currentDiagram->simplify(2)) {
		this->addCommand(simplifyCommand, true);
		return true;
	}
	return false;
}

namespace {

struct CaptionCommand : public DiagramEditor::Command {
	const std::string caption;

	CaptionCommand(const std::string &caption) : caption(caption) {}

	void play(Diagram &diagram) override {
		diagram.caption = caption;
	}
};

}
void DiagramEditor::setCaption(const std::string &caption) {
	if (caption != this->currentDiagram->caption) {
		this->addCommand(std::make_shared<CaptionCommand>(caption), true);
		this->currentDiagram->caption = caption;
	}
}

}}
