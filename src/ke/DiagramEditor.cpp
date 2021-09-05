/*
 * Copyright (c) 1995-2021, Nikolay Pultsin <geometer@geometer.name>
 *
 * Licensed under the Apache License, Version 2.0 the "License";
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "DiagramEditor.h"

namespace KE::TwoD {

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
	const int x, y;

	MoveDiagramCommand(int x, int y) : x(x), y(y) {}

	void play(Diagram &diagram) override {
		const auto &first = diagram.vertices().front();
		diagram.shift(x - first->x(), y - first->y());
	}
};

}

void DiagramEditor::shift(int dx, int dy, bool storeCommand) {
	this->currentDiagram->shift(dx, dy);
	if (storeCommand && !this->currentDiagram->vertices().empty()) {
		const auto &first = this->currentDiagram->vertices().front();
		this->addCommand(std::make_shared<MoveDiagramCommand>(first->x(), first->y()), false);
	}
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

}
